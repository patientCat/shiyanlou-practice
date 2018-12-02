//c++ program
//design by >^.^<
//@date    2018-11-07 17:50:34

#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAX_ERROR_LEN 256

static char buf[MAX_ERROR_LEN];

char *mystrerror(int err)
{
    /* 如果err为负，或者超出系统定义的最大值，或者没有定义 */
    if(err < 0 || err >= sys_nerr || NULL == sys_errlist[err])
        snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
    else{
        // 否则依据err，在sys_errlist中找到错误码描述
        strncpy(buf, sys_errlist[err], MAX_ERROR_LEN); 
        // 确保字符串以0结束
        buf[MAX_ERROR_LEN - 1] = 0;
    }
    return buf;
}

