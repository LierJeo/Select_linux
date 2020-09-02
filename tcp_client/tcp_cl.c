/*
 * @Author: your name
 * @Date: 2020-08-31 11:43:03
 * @LastEditTime: 2020-09-02 13:57:32
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /select/tcp_client/tcp_cl.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../common/common.h"
#include <sys/uio.h>
//#define PORT 9527

void process_client(int socket);
struct iovec v;

int main()
{
	int tcp_socket;
	struct sockaddr_in server_addr;
	tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp_socket < 0)
	{
		printf("socket error: %d\n", errno);
		return -1;
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(LISTEN_PORT);

	//inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

	if (-1 == connect(tcp_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)))
	{
		//printf("connect error:%d\n", errno);
		perror("connect error");
		return -1;
	}
	process_client(tcp_socket);
	return 0;
}

void process_client(int socket)
{
	char name[] = "tcp client";
	struct Massage message;
	message.count = 0;
	memcpy(message.client_name, name, sizeof(message.client_name) > sizeof(name) ? sizeof(name) : sizeof(message.client_name));
	v.iov_base = &message;
	v.iov_len = sizeof(message);
	while (1)
	{
		message.count += 1;
		writev(socket, &v, 1);
		printf("send message:\n %d  %s\n", message.count, message.client_name);
		//size = read(socket, buffer, sizeof(buffer));
		sleep(3);
	}
}