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
 * @LastEditTime: 2020-09-06 01:59:04
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
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "common.h"
#include "log.h"

#define SERVER_T2 10
#define SERVER_MAX_CLIENT 2


//#define udp_listen_socket 	client[1].client_socket
#define udp_client client[1]
#define tcp_client client[0]
int t2 = 10;

typedef struct CLIENT
{
	int client_socket;
	int sock_type;
	int alive_count;
	time_t alive_time;
	struct sockaddr_in client_addr;
} Client;

void reset_client(Client *client);
char *socktypetostr(int type);
int memcpy_st(void *det, size_t detSize, const void *src, size_t srcSize, char *cppName, uint32_t lineNumber);

int main()
{
	int client_count = 0;
	int tcp_listen_socket = 0;
	int udp_listen_socket = 0;
	Client client[SERVER_MAX_CLIENT];
	for (int i = 0; i < SERVER_MAX_CLIENT; i++)
	{
		reset_client(&client[i]);
	}
	
	struct Massage message;
	struct sockaddr_in serv_addr;

	fd_set ndfs;	//文件描述符最大值+1
	fd_set tm;		//变化的套接字描述符
	int sl_ret = 0; //select 返回值

	char recvbuf[SELECT_MAX_SIZE] = {0};
	struct timeval tv = {0, 0};
	time_t left_time = 0;
	char ipbuf[20] = {0};

	//创建监听套接字
	tcp_listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	udp_listen_socket = socket(AF_INET, SOCK_DGRAM, 0);

	if (tcp_listen_socket == -1 || udp_listen_socket == -1)
	{
		SELECT_LOGWRITE(LOG_ERROR,"%s","listen socket create error");
		goto END;
	}

	memset((void *)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(SELECT_LISTEN_PORT);

	if ((-1 == bind(tcp_listen_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) ||
		(-1 == bind(udp_listen_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr))))
	{
		SELECT_LOGWRITE(LOG_ERROR, "%s", "bind error");
		goto END;
	}
	
	listen(tcp_listen_socket, SELECT_MAX_LINE);
	SELECT_LOGWRITE(LOG_INFO, "start listen.SERVER IP:%s listen port:%d.",
			 inet_ntop(AF_INET, &serv_addr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
			 ntohs(serv_addr.sin_port));
	int maxfd = tcp_listen_socket > udp_listen_socket ? tcp_listen_socket : udp_listen_socket;
	FD_ZERO(&ndfs);
	FD_SET(tcp_listen_socket, &ndfs);
	FD_SET(udp_listen_socket, &ndfs);

	while (1)
	{
		tm = ndfs;	
		//计时归零时重置
		if (tv.tv_sec <= 0)
		{
			tv.tv_sec = SELECT_ALIVE_TIME;
		}
		left_time = tv.tv_sec;
		// //计算周期t2剩余时间
		// t2 -= (left_time - tv.tv_sec);
		// if (0 < t2 && t2 <= 3)
		// {
		// 	tv.tv_sec = t2;
		// }
		// else if (t2 <= 0)
		// {
		// 	//一个t2周期
		// 	t2 = SERVER_T2;
		// 	SELECT_LOGWRITE(LOG_DEBUG, "T2: client count:%d", client_count);
		// 	if (client_count > 0)
		// 	{
		// 		for (int i = 0; i < SERVER_MAX_CLIENT; i++)
		// 		{
		// 			if (client[i].alive_count != 0)
		// 			{
		// 				char ipbuf[20];
		// 				SELECT_SOCK_INFO("T2 printf:",client[i].client_addr.sin_addr.s_addr,
		// 								 client[i].client_addr.sin_port,
		// 								 client[i].alive_count,
		// 								 ipbuf);
		// 			}
		// 		}
		// 	}
		// 	else
		// 	{
		// 		SELECT_LOGWRITE(LOG_INFO, "T2 printf:no client");
		// 	}
		// }

		sl_ret = select(maxfd + 1, &tm, NULL, NULL, &tv);
		if (sl_ret == -1)
		{
			SELECT_LOGWRITE(LOG_ERROR, "no client");
			goto END;
		}
		//计算周期t2剩余时间
		t2 -= (left_time - tv.tv_sec);
		if (0 < t2 && t2 <= 3)
		{
			tv.tv_sec = t2;
		}
		else if (t2 <= 0)
		{
			//一个t2周期
			t2 = SERVER_T2;
			SELECT_LOGWRITE(LOG_DEBUG, "T2: client count:%d", client_count);
			if (client_count > 0)
			{

				for (int i = 0; i < SERVER_MAX_CLIENT; i++)
				{
					if (client[i].alive_count > 0)
					{
						char ipbuf[20];
						SELECT_SOCK_INFO("T2 printf:", client[i].client_addr.sin_addr.s_addr,
										 client[i].client_addr.sin_port,
										 client[i].alive_count,
										 socktypetostr(client[i].sock_type),
										 ipbuf);
					}
				}
			}
			else
			{
				SELECT_LOGWRITE(LOG_INFO, "T2 printf:no client");
			}
		}
		if (FD_ISSET(tcp_listen_socket, &tm))
		{
			socklen_t clien_len = sizeof(struct sockaddr);
			tcp_client.client_socket = accept(tcp_listen_socket, (struct sockaddr *)&tcp_client.client_addr, &clien_len);
			if (tcp_client.client_socket == -1)
			{
				SELECT_LOGWRITE(LOG_ERROR,"accept error");
				continue;
			}
			client_count += 1;
			SELECT_LOGWRITE(LOG_DEBUG, "accept tcpclient: socket type:%s. alive time:%d", socktypetostr(tcp_client.sock_type),tcp_client.alive_time);
			tcp_client.sock_type = SOCK_STREAM;
			FD_SET(tcp_client.client_socket, &ndfs);
			//重新设置最大描述符；
			maxfd = maxfd < tcp_client.client_socket ? tcp_client.client_socket : maxfd;
		}
		if (FD_ISSET(udp_listen_socket, &tm))
		{
			if (udp_client.client_socket == -1)
			{
				udp_client.client_socket = udp_listen_socket;
				udp_client.sock_type = SOCK_DGRAM;
				SELECT_LOGWRITE(LOG_DEBUG, "accept udpclient: client count:%d", client_count);
				client_count += 1;
			}
		}
		for (int i = 0; i < SERVER_MAX_CLIENT; i++)
		{
			if (FD_ISSET(client[i].client_socket, &tm))
			{
				memset(recvbuf, 0, sizeof(recvbuf));
				struct sockaddr_in cl_addr;
				socklen_t clien_len = sizeof(cl_addr);
				int recv_len = recvfrom(client[i].client_socket, recvbuf, sizeof(recvbuf),
										0, (struct sockaddr *)&client[i].client_addr, &clien_len);
				if (recv_len == -1)
				{
					SELECT_LOGWRITE(LOG_ERROR, "recv error");
					continue;
				}
				else if (recv_len == 0)
				{
					SELECT_SOCK_INFO("client disconnect: ", client[i].client_addr.sin_addr.s_addr,
									 client[i].client_addr.sin_port,
									 client[i].alive_count,
									 socktypetostr(client[i].sock_type),
									 ipbuf);
					FD_CLR(client[i].client_socket, &ndfs);
					close(client[i].client_socket);
					reset_client(&client[i]);
					client_count -= 1;
				}
				else
				{
					client[i].alive_time = 0;
					client[i].alive_count += 1;
					memcpy(&message, recvbuf, sizeof(message));
					SELECT_LOGWRITE(LOG_INFO, "message count:%d. client name:%s", message.count, message.client_name);
				}
			}
			else
			{
				if (client[i].client_socket != -1)
				{
					client[i].alive_time += left_time - tv.tv_sec;
					SELECT_LOGWRITE(LOG_DEBUG, "socket type:%s. alive_time:%d", socktypetostr(client[i].sock_type),client[i].alive_time);
					if (client[i].alive_time >= 3 * SELECT_ALIVE_TIME)
					{
						SELECT_SOCK_INFO("out of time: ", client[i].client_addr.sin_addr.s_addr,
										 client[i].client_addr.sin_port,
										 client[i].alive_count,
										 socktypetostr(client[i].sock_type),
										 ipbuf);
						if (client[i].sock_type == SOCK_STREAM)
						{
							close(client[i].client_socket);
							FD_CLR(client[i].client_socket, &ndfs);
							reset_client(&client[i]);
							client_count -= 1;
						}
						else
						{
							//FD_CLR(client[i].client_socket, &ndfs);
							reset_client(&client[i]);
							client_count -= 1;
						}
					}
				}//if (client[i].client_socket != -1)
			}//if (FD_ISSET(client[i].client_socket, &tm))
		}//for (int i = 0; i < SERVER_MAX_CLIENT; i++)
	}//while (1)

END:
	if (tcp_listen_socket < 0)
	{
		close(tcp_listen_socket);
	}
	if (udp_listen_socket < 0)
	{
		close(udp_listen_socket);
	}
	for (int i = 0; i < SERVER_MAX_CLIENT; i++)
	{
		if (client[i].client_socket > 0)
		{
			close(client[i].client_socket);
		}
	}

	return 0;
}


void reset_client(Client *client)
{
	memset(client, 0, sizeof(Client));
	client->client_socket = -1;
}

char *socktypetostr(int type)
{
	if(type == SOCK_STREAM)
	{
		return "SOCK_STREAM";
	}
	else if (type == SOCK_DGRAM)
	{
		return "SOCK_DGRAM";
	}
	else
	{
		return "OTHERS";
	}
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