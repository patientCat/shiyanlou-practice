//c++ program
//design by >^.^<
//@date    2018-10-18 21:38:30

#include "common.h"

int main()
{
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = PF_INET; //AF_INET对应于BSD标准, PF_INET对应于Posix标准
    serverAddr.sin_port = htons(SERVER_PORT); // 设置端口号，主机字节序需要转网络序
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_HOST); // 设置ip地址
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    Perror(sockfd, 0, "socket error\n");

    int ret_bind = bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(struct sockaddr));
    Perror(ret_bind, 0, "bind error\n");

    int ret_listen = listen(sockfd, 5);
    Perror(ret_listen, 0, "listen error\n");

    int clientfd = accept(sockfd, NULL, NULL);
    Perror(clientfd, 0, "accept error\n");

    char buff[BUFFSIZE] = {0};

    int ret_recv = recv(clientfd, buff, BUFFSIZE - 1, 0);
    write(STDOUT_FILENO, buff, ret_recv);
    memset(buff, 0, BUFFSIZE);
    strcpy(buff, "hello im server\n");
    send(clientfd, buff, strlen(buff), 0); 
    return 0;
}
