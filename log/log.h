/*
 * @Author: your name
 * @Date: 2020-09-04 09:11:22
 * @LastEditTime: 2020-09-04 19:19:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /demo/log/log.h
 */



#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#define MAXLEN (2048)
#define MAXFILEPATH (512)
#define MAXFILENAME (50)
typedef enum
{
	ERROR_1 = -1,
	ERROR_2 = -2,
	ERROR_3 = -3
} ERROR0;

typedef enum
{
	LOG_NONE = 0,
	LOG_INFO = 1,
	LOG_DEBUG = 2,
	LOG_WARN = 3,
	LOG_ERROR = 4,
	LOG_ALL = 255
} LOGLEVEL;

typedef struct log
{
	char logtime[20];
	char filepath[MAXFILEPATH];
	FILE *logfile;
} LOG;

typedef struct logseting
{
	char filepath[MAXFILEPATH];
	unsigned int maxfilelen;
	unsigned char loglevel;
} LOGSET;

int LogWrite(unsigned char loglevel, char *fromat, ...);

//#define CHARCAT(a) "[FILE]:%s:%d:@"a
// #define SELECT_LOGWRITE(log_level, fmt, x...) 
// LogWrite(log_level, fmt "\n", ##x)

#define SELECT_LOGWRITE(log_level, fmt, x...) \
LogWrite(log_level, "[FILE]%s:%d " fmt "\n", __FILE__, __LINE__, ##x)

#define SELECT_SOCK_INFO(fmt, sock_ip, sock_port, sock_alive_count, ipbuf)                        					\
LogWrite(LOG_INFO, "[FILE]:%s:%d:" fmt "client: IP:%s. port:" #sock_port ". alive count:" #sock_alive_count "\n",	\
		__FILE__, __LINE__,inet_ntop(AF_INET, &sock_ip, ipbuf, sizeof(ipbuf)))

#endif /* __LOG_H__ */
