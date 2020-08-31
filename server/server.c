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
 *        佛曰:  
 *                写字楼里写字间，写字间里程序员；  
 *                程序人员写程序，又拿程序换酒钱。  
 *                酒醒只在网上坐，酒醉还来网下眠；  
 *                酒醉酒醒日复日，网上网下年复年。  
 *                但愿老死电脑间，不愿鞠躬老板前；  
 *                奔驰宝马贵者趣，公交自行程序员。  
 *                别人笑我忒疯癫，我笑自己命太贱；  
 *                不见满街漂亮妹，哪个归得程序员？
 * 
 * @Author: your name
 * @Date: 2020-08-28 17:19:07
 * @LastEditTime: 2020-08-31 16:58:10
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /select/server/server.c
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

//#define OPEN_MAX 1024
#define LISTEN_PORT 9527
#define MAX_SIZE 1024
#define MAX_LINE 64

int memcpy_st(void *det, size_t detSize, const void *src, size_t srcSize, char *cppName, uint32_t lineNumber);

int main()
{
    int tcp_listen_socket;
    int udp_listen_socket;
    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;

    extern int errno;

    fd_set ndfs;            //文件描述符最大值+1
    fd_set tm;              //变化的套接字描述符
    int sl_ret;             //select 返回值

    char recvbuf[MAX_SIZE] = {0};

    //const char sendchar[] = "server receive:";

    //创建监听套接字
    tcp_listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    udp_listen_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (tcp_listen_socket == -1 || udp_listen_socket == -1)
    {
        perror("listen socket create error");
    }

    memset((void *)&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(LISTEN_PORT);

    //int opt = 1;
    //setsockopt(tcp_listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if(-1 == bind(tcp_listen_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
    {
        perror("bind error");
    }
    if(-1 == bind(udp_listen_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
    {
        perror("bind error");
    }
    
    listen(tcp_listen_socket, MAX_LINE);
    printf("start listen...\n");
    char ipbuf[16] = {0};
    printf("IP:%s \nlisten port:%d\n",
           inet_ntop(AF_INET, &serv_addr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
           ntohs(serv_addr.sin_port));

    int maxfd = tcp_listen_socket > udp_listen_socket ? \
                tcp_listen_socket : udp_listen_socket;

    
    FD_ZERO(&ndfs);
    FD_SET(tcp_listen_socket, &ndfs);
    FD_SET(udp_listen_socket, &ndfs);

    while (1)
    {
        tm = ndfs;
        sl_ret = select(maxfd + 1, &tm, NULL, NULL, NULL);
        if (sl_ret == -1)
        {
            perror("select error");
            exit(1);
        }
        if (FD_ISSET(tcp_listen_socket, &tm))   
        {
            socklen_t clien_len = sizeof(client_addr);
            int tcp_socket = accept(tcp_listen_socket, (struct sockaddr *)&client_addr, &clien_len);
            if(tcp_socket == -1)
            {
                perror("accept error");
                continue;
            }
            char ipbuf[16] = {0};
            printf("new client link \n \
                    IP: %s\n \
                    port: %d\n",
                   inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
                   ntohs(client_addr.sin_port));
            FD_SET(tcp_socket, &ndfs);
            FD_CLR(tcp_listen_socket, &tm);
            //重新设置最大描述符；
            maxfd = maxfd < tcp_socket ? tcp_socket : maxfd;
        }

        for (int i = 0; i <= maxfd; i++)
        {
            if (FD_ISSET(i, &tm))
            {
                memset(recvbuf, 0, sizeof(recvbuf));
                int recv_len = (int)read(i, recvbuf, sizeof(recvbuf));
                if (recv_len == -1)
                {
                   perror("recv error");
                   continue;
                }else if (recv_len == 0)
                {
                    struct sockaddr_in addr;
                    char ipbuf[16] = {0};
                    socklen_t sockaddr_len = sizeof(addr);
                    getpeername(i, (struct sockaddr *)&addr, &sockaddr_len);
                    printf("client log out. IP:%s\n",
                           inet_ntop(AF_INET, &addr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)));
                    FD_CLR(i, &ndfs);                   
                }
                else
                {
                    printf("read buf: %s", recvbuf);
                    char sendchar[] = "server receive: ";
                    char *sendbuf = strcat(sendchar, recvbuf);
                    printf("%s", sendbuf);
                    send(i, sendbuf, strlen(sendbuf), 0);
                }
            }
        }        
    }
    return 0;
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