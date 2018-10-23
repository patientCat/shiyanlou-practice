//c++ program
//design by >^.^<
//@date    2018-10-18 21:56:54

#include "common.h"

int main()
{
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = PF_INET; //AF_INET对应于BSD标准, PF_INET对应于Posix标准
    serverAddr.sin_port = htons(SERVER_PORT); // 设置端口号，主机字节序需要转网络序
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_HOST); // 设置ip地址
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    Perror(sockfd, 0, "socket error\n");

    int ret_conn = connect(sockfd, (struct sockaddr*) &serverAddr, sizeof(struct sockaddr));
    Perror(ret_conn, 0, "connect error\n");

    char buff[BUFFSIZE] = {0};
    strcpy(buff, "hello im client\n");
    send(sockfd, buff, strlen(buff), 0);
    memset(buff, 0, BUFFSIZE);
    int ret_recv = recv(sockfd, buff, BUFFSIZE - 1, 0);
    write(STDOUT_FILENO, buff, ret_recv);
    return 0;
}
