//c++ program
//design by >^.^<
//@date    2018-11-10 23:23:18

#include "utility.hh" 

#include <sys/socket.h> 
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <strings.h> 

#define errHandle(msg) \
    do{ \
        fprintf(stderr, "%s\n", msg); \
        exit(EXIT_FAILURE); \
    }while(0);

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT "8888"
#define BUFFSIZE 1024

void processtask(int fd)
{
    // 业务处理
    int count;
    // 几个数字
    char buff[BUFFSIZE];
    printf("Operand count:");
    int offset = 0;
    scanf("%d", (int *)&buff[0]);
    offset += 4;
    
    printf("\n");
    // 发送这些数字
    for(int i = 0; i < (int); ++i)
    {
        printf("Operand %d: ", i+1);
        scanf("%d", (int *)(buff + offset));
        offset += 4;
        printf("\n");
    }
    // 哪个运算方法
    rewind(stdin);
    char operSymbol = fgetc(stdin);
    sendtrain(fd, &operSymbol, sizeof(operSymbol), 0);

    // 从服务器得到计算结果
    long ret;
    recvtrain(fd, &ret, 0);
    fprintf(stdout, "the result is %ld\n", ret);
}

int main()
{
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock < 0)
        errHandle("socket");
    struct sockaddr_in sveraddr;
    sveraddr.sin_port = htons(atoi(SERVER_PORT));
    sveraddr.sin_family = AF_INET;
    sveraddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    int ret = connect(sock, (struct sockaddr *)&sveraddr, sizeof(sveraddr));
    if(ret < 0)
        errHandle("connect");

    processtask(sock);
    return 0;
}


