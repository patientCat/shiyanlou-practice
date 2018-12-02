//c++ program
//design by >^.^<
//@date    2018-11-07 17:59:12

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

extern char *mystrerror(int err);

static void *threadFunc(void *arg)
{
    char *str;
    fprintf(stdout, "Other thread about to call mystrerror()\n");

    str = mystrerror(EPERM);
    printf("Other thread : str (%p) = %s\n", str, str);
    pthread_exit(NULL);
}

int main()
{
    pthread_t t;
    char *str;
    // 主线程获取EINVAL错误码的字符信息
    str = mystrerror(EINVAL);
    printf("Main thread has called mystrerror()\n");
    pthread_create(&t, NULL, threadFunc, NULL);
    pthread_join(t, NULL);
    printf("Main thread: str (%p) = %s\n", str, str);
    return 0;
}
