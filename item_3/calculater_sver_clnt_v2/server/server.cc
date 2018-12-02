//c++ program
//design by >^.^<
//@date    2018-11-10 22:59:35

#include "../include/utility.hh" 

#include <sys/socket.h> 
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <strings.h> 

#define SERVER_PORT "8888"

#define errHandle(msg) \
    do{ \
        fprintf(stderr, "%s\n", msg); \
        exit(EXIT_FAILURE); \
    }while(0);

long calculate(int *arr, int num, char oper)
{
    long ret = 0;
    switch(oper)
    {
        case '+':
            for(int i = 0; i < num; ++i)
                ret += arr[i];
            break;
        case '-':
            ret = arr[0];
            for(int i = 1; i < num; ++i)
                ret -= arr[i];
            break;
        case '*':
            ret = 1;
            for(int i = 0; i < num; ++i)
                ret *= arr[i];
            break;
        default:
            fprintf(stderr, "wrong operational symbol\n");
            break;
    }
    return ret;
}

void processTask(int fd)
{
    // 先收有几个数字
    int num = 0;
    recvtrain(fd, &num, 0);
    printf("num : %d\n", num);

    int *arr = (int *)calloc(num, sizeof(sizeof(int)));
    // 收到数字并存储
    for(int i = 0; i < num; ++i)
        recvtrain(fd, &arr[i], 0);
    // 收到运算符
    char operSymbol;
    recvtrain(fd, &operSymbol, 0);
    // 服务器计算结果
    long ret = calculate(arr, num, operSymbol);
    // 服务器返回计算结果
    sendtrain(fd, &ret, sizeof(ret), 0);
}

int main()
{
    int sverfd, clntfd;
    int ret;
    sverfd = socket(PF_INET, SOCK_STREAM, 0);
    if(sverfd < 0)
        errHandle("socket");
    printf("sfd socket successfully\n");
    struct sockaddr_in sveraddr;
    bzero(&sveraddr, sizeof(sveraddr));
    sveraddr.sin_family = AF_INET;
    sveraddr.sin_port = htons(atoi(SERVER_PORT));
    sveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(sverfd, (struct sockaddr *)&sveraddr, sizeof(sveraddr));
    if(ret < 0)
        errHandle("listen");
    printf("sfd bind successfully\n");

    ret = listen(sverfd, 5);
    if(ret < 0)
        errHandle("listen");
    printf("sfd listen successfully\n");

    struct sockaddr_in clntaddr;
    socklen_t clntsize;
    while(1)
    {
        clntfd = accept(sverfd, (struct sockaddr *)&clntaddr, &clntsize);
        printf("sfd accept successfully\n");
        if(clntfd < 0)
            errHandle("accept");
        // 按照协议办事
        processTask(clntfd);
        close(clntfd);
    }

    close(sverfd);

    return 0;
}
