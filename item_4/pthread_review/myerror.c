//c++ program
//design by >^.^<
//@date    2018-11-07 17:37:34

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t strerrorKey;

#define handleError(errnumber, msg) \
    do {    errno = errnumber; perror(msg); exit(EXIT_FAILURE); }while(0);
#define MAX_ERROR_LEN 256

// 线程特有数据的析构函数
static void destructor(void *buf)
{
    free(buf);
}

static void createKey(void)
{
    int s;
    // pthread_once函数里创建特有数据的key，哪个线程先调用就哪个线程创建key
    s = pthread_key_create(&strerrorKey, destructor);
    if(0 != s)
        handleError(s, "pthread_key_create");
}

char *mystrerror(int err)
{
    char *buf;
    
    pthread_once(&once, createKey);

    // 获取线程特有数据
    buf = (char *)pthread_getspecific(strerrorKey);
    if(NULL == buf)
    {
        buf = (char *)malloc(MAX_ERROR_LEN);
        
        pthread_setspecific(strerrorKey, buf);
    }

    if (err < 0 || err >= sys_nerr || sys_errlist[err] == NULL) {
        snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
    } else {
        strncpy(buf, sys_errlist[err], MAX_ERROR_LEN - 1);
        buf[MAX_ERROR_LEN - 1] = '\0'; /* Ensure null termination */
    }

    return buf;
}


