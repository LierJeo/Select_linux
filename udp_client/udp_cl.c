/*
 * @Author: your name
 * @Date: 2020-09-02 10:35:12
 * @LastEditTime: 2020-09-04 17:06:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /select/udp_client/ucp_cl.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "common.h"
#include <sys/uio.h>

void process_client(int socket, struct sockaddr_in addr_serv);

int main()
{
	int udp_socket;
	struct sockaddr_in addr_serv;

	udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&addr_serv, 0, sizeof(addr_serv));
	addr_serv.sin_family = AF_INET;
	addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_serv.sin_port = htons(SELECT_LISTEN_PORT);

	process_client(udp_socket, addr_serv);
	close(udp_socket);

	return 0;
}

void process_client(int socket, struct sockaddr_in addr_serv)
{
	char name[] = "ucp client";
	struct Massage massage;
	massage.count = 0;
	memcpy(massage.client_name, name, sizeof(name));

	while (1)
	{
		massage.count += 1;
		sendto(socket, &massage, sizeof(massage), 0, (struct sockaddr*)&addr_serv, sizeof(addr_serv));
		printf("send message:\n %d  %s\n", massage.count, massage.client_name);
		sleep(3);
	}
}