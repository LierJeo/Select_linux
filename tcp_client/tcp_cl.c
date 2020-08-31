/*
 * @Author: your name
 * @Date: 2020-08-31 11:43:03
 * @LastEditTime: 2020-08-31 17:01:09
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
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern int errno;

#define PORT 9527
#define MAX_SIZE 1024

void process_client(int socket);

int main()
{
    int tcp_socket;
    struct sockaddr_in server_addr;
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(tcp_socket < 0)
    {
        printf("socket error: %d\n", errno);
        return -1;
    }
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    //inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    if(-1 == connect(tcp_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)))
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
    ssize_t size = 0;
    char buffer[MAX_SIZE] = {0};

    while(1)
    {
        size = read(0, buffer, sizeof(buffer));
        if(size > 0)
        {
            write(socket, buffer, size);
            size = read(socket, buffer, MAX_SIZE);
            write(1, buffer, size);
        }
    }
}