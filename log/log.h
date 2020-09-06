/*
 * @Author: your name
 * @Date: 2020-09-04 09:11:22
 * @LastEditTime: 2020-09-06 01:53:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /demo/log/log.h
 */

#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
//#define MAXLEN (2048)
//#define MAXFILEPATH (512)
//#define MAXFILENAME (50)

typedef enum
{
	LOG_NONE = 0b00000000,
	LOG_ERROR = 0b00000001,
	LOG_WARNING = 0b00000011,
	LOG_INFO = 0b00000111,
	LOG_DEBUG = 0b00001111,
	LOG_ALL = 255
} LOGLEVEL;

typedef struct log
{
	char logtime[20];
	//char filepath[MAXFILEPATH];
	FILE *logfile;
} LOG;

typedef struct logseting
{
	//char filepath[MAXFILEPATH];
	//unsigned int maxfilelen;
	unsigned char loglevel;
} LOGSET;

int LogWrite(unsigned char loglevel, char *fromat, ...);

//#define CHARCAT(a) "[FILE]:%s:%d:@"a
// #define SELECT_LOGWRITE(log_level, fmt, x...)
// LogWrite(log_level, fmt "\n", ##x)

#define SELECT_LOGWRITE(log_level, fmt, x...) \
LogWrite(log_level, "[FILE]%s:%d " fmt "\n", __FILE__, __LINE__, ##x)

#define SELECT_SOCK_INFO(str, sock_ip, sock_port, sock_alive_count, sock_type, ipbuf)                                     \
	LogWrite(LOG_INFO, "[FILE]%s:%d:" str "client: IP:%s. port:%d. alive count:%d. socket type:%s\n", __FILE__, __LINE__, \
			 inet_ntop(AF_INET, &sock_ip, ipbuf, sizeof(ipbuf)), sock_port, sock_alive_count, sock_type)

#endif /* __LOG_H__ */
