/*
 * @Author: your name
 * @Date: 2020-08-31 19:13:21
 * @LastEditTime: 2020-09-03 09:07:59
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /select/common/common.h
 */
#ifndef _COMMON_
#define _COMMON_

#include <errno.h>

#define SELECT_LISTEN_PORT 9527
#define SELECT_MAX_SIZE 1024
#define SELECT_MAX_LINE 64
extern int errno;

#define SELECT_ALIVE_TIME 3

struct Massage
{
    int count;
    char client_name[50];
};

#endif // !_COMMON_
