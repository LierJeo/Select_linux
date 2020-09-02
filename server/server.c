/*
 *                        _oo0oo_
 *                       o8888888o
 *                       88" . "88
 *                       (| -_- |)
 *                       0\  =  /0
 *                     ___/`---'\___
 *                   .' \\|     |// '.
 *                  / \\|||  :  |||// \
 *                 / _||||| -:- |||||- \
 *                |   | \\\  - /// |   |
 *                | \_|  ''\---/''  |_/ |
 *                \  .-\__  '-'  ___/-. /
 *              ___'. .'  /--.--\  `. .'___
 *           ."" '<  `.___\_<|>_/___.' >' "".
 *          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *          \  \ `_.   \_ __\ /__ _/   .-` /  /
 *      =====`-.____`.___ \_____/___.-`___.-'=====
 *                        `=---='
 * 
 * 
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *            佛祖保佑       永不宕机     永无BUG
 * 
 * @Author: your name
 * @Date: 2020-08-28 17:19:07
 * @LastEditTime: 2020-09-02 15:10:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /select/server/server.c
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include "../common/common.h"
#include <time.h>

#define T2 10

int t2 = 10;

typedef struct CLIENT
{
	int client_socket;
	int aliva_count;
	time_t alive_time;
	struct sockaddr_in client_addr;
} Client;

void printf_so(Client client);
void reset_client(Client *client);
void printf_time_func(const char *str);
int memcpy_st(void *det, size_t detSize, const void *src, size_t srcSize, char *cppName, uint32_t lineNumber);

int main()
{

	int tcp_listen_socket = 0;
	int udp_listen_socket = 0;

	Client tcp_client;
	Client udp_client;
	reset_client(&tcp_client);
	reset_client(&udp_client);

	struct Massage message;
	struct sockaddr_in serv_addr;

	fd_set ndfs;	//文件描述符最大值+1
	fd_set tm;		//变化的套接字描述符
	int sl_ret = 0; //select 返回值

	char recvbuf[MAX_SIZE] = {0};
	struct iovec v;
	v.iov_base = &recvbuf;
	v.iov_len = sizeof(recvbuf);

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	//创建监听套接字
	tcp_listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	udp_listen_socket = socket(AF_INET, SOCK_DGRAM, 0);

	if (tcp_listen_socket == -1 || udp_listen_socket == -1)
	{
		printf_time_func(__func__);
		perror("listen socket create error");
	}

	memset((void *)&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(LISTEN_PORT);

	if ((-1 == bind(tcp_listen_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) ||
		(-1 == bind(udp_listen_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr))))
	{
		printf_time_func(__func__);
		perror("bind error");
	}

	listen(tcp_listen_socket, MAX_LINE);
	printf_time_func(__func__);
	printf("start listen...  ");
	char ipbuf[16] = {0};
	printf("SERVER IP:%s   listen port:%d\n",
		   inet_ntop(AF_INET, &serv_addr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
		   ntohs(serv_addr.sin_port));

	int maxfd = tcp_listen_socket > udp_listen_socket ? tcp_listen_socket : udp_listen_socket;

	FD_ZERO(&ndfs);
	FD_SET(tcp_listen_socket, &ndfs);
	FD_SET(udp_listen_socket, &ndfs);

	while (true)
	{
		tm = ndfs;
		time_t left_time = tv.tv_sec;
		//计时归零时重置
		if (tv.tv_sec == 0)
		{
			tv.tv_sec = ALIVE_TIME;
		}
		//计算周期t2剩余时间
		t2 -= (ALIVE_TIME - left_time);
		if (0 < t2 && t2 <= 3)
		{
			tv.tv_sec = t2;
		}
		else if (t2 <= 0)
		{
			//一个t2周期
			t2 = T2;
			printf_time_func(__func__);
			printf("T2 printf:  ");
			if (tcp_client.client_socket != -1)
			{
				printf_so(tcp_client);
			}
			else
			{
				printf_time_func(__func__);
				printf("no client.\n");
			}
		}

		sl_ret = select(maxfd + 1, &tm, NULL, NULL, &tv);
		if (sl_ret == -1)
		{
			printf_time_func(__func__);
			perror("select error");
			return -1;
		}
		if (FD_ISSET(tcp_listen_socket, &tm))
		{
			socklen_t clien_len = sizeof(tcp_client.client_addr);
			tcp_client.client_socket = accept(tcp_listen_socket, (struct sockaddr *)&tcp_client.client_addr, &clien_len);
			if (tcp_client.client_socket == -1)
			{
				printf_time_func(__func__);
				perror("accept error");
				continue;
			}
			printf_so(tcp_client);
			FD_SET(tcp_client.client_socket, &ndfs);
			//重新设置最大描述符；
			maxfd = maxfd < tcp_client.client_socket ? tcp_client.client_socket : maxfd;
		}
		if (tcp_client.client_socket != -1)
		{
			if (FD_ISSET(tcp_client.client_socket, &tm))
			{
				tcp_client.alive_time = 0;
				memset(recvbuf, 0, sizeof(recvbuf));
				int recv_len = (int)readv(tcp_client.client_socket, &v, 1);
				if (recv_len == -1)
				{
					printf_time_func(__func__);
					perror("recv error");
					continue;
				}
				else if (recv_len == 0)
				{
					printf_time_func(__func__);
					printf("  client disconnect:  ");
					printf_so(tcp_client);
					FD_CLR(tcp_client.client_socket, &ndfs);
					close(tcp_client.client_socket);
					reset_client(&tcp_client);
				}
				else
				{
					tcp_client.aliva_count += 1;
					memcpy(&message, recvbuf, sizeof(message));
					printf_time_func(__func__);
					printf_so(tcp_client);
					printf("\tcount:%d client name:%s\n", message.count, message.client_name);
					// message.count += 1;
					// v.iov_len = sizeof(message);
					// v.iov_base = &message;
					// writev(tcp_client.client_socket, &v, 1);
				}
			}
			else
			{
				tcp_client.alive_time += ALIVE_TIME - left_time;
				if (tcp_client.alive_time >= 3 * ALIVE_TIME)
				{
					close(tcp_client.client_socket);
					FD_CLR(tcp_client.client_socket, &ndfs);
					tcp_client.client_socket = -1;
				}
			}
		}
		if (FD_ISSET(udp_listen_socket, &tm))
		{
			memset(recvbuf, 0, sizeof(recvbuf));
			socklen_t clien_len = sizeof(udp_client.client_addr);
			int recv_len = recvfrom(udp_listen_socket, recvbuf, sizeof(recvbuf),
									0, (struct sockaddr *)&(udp_client.client_addr), &clien_len);
			if (recv_len == -1)
			{
				printf_time_func(__func__);
				perror("recv error");
				continue;
			}
			memcpy(&message, recvbuf, sizeof(message));
			printf_time_func(__func__);
			printf_so(udp_client);
			printf("\tcount: %d  client name: %s\n\t", message.count, message.client_name);
		}
	}
	return 0;
}

void printf_so(Client client)
{
	char ipbuf[16] = {0};
	printf("client:  IP:%s  port:%d  alive count:%d\n",
		   inet_ntop(AF_INET, &client.client_addr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
		   ntohs(client.client_addr.sin_port),
		   client.aliva_count);
}

void printf_time_func(const char *str)
{
	printf("[Time]%s. Fucntion%s: ", __TIME__, str);
}

void reset_client(Client *client)
{
	memset(client, 0, sizeof(Client));
	client->client_socket = -1;
}


int memcpy_st(void *det, size_t detSize, const void *src, size_t srcSize, char *cppName, uint32_t lineNumber)
{
	uint8_t errorcode = 0;
	if (srcSize > detSize || src == NULL || det == NULL)
	{
		if (srcSize > detSize)
			errorcode = 1;
		else if (src == NULL)
			errorcode = 2;
		else if (det == NULL)
			errorcode = 3;
		//printf("[waring] %s.%d memcpy_s is error:%d!\n", cppName, lineNumber, errorcode);
		printf("[waring] %s.%d memcpy_s is error:%d [%ld---%ld]!\n", cppName, lineNumber, errorcode, srcSize, detSize);
		fflush(stdout);
		return -1;
	}
	else
		memcpy(det, src, srcSize);

	return 1;
}