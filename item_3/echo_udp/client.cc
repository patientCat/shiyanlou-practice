//c++ program
//design by >^.^<
//@date    2018-11-17 20:47:37

// server

#include "head.hh" 

#define errExit(msg) \
    do{\
        fprintf(stderr, "%s\n", msg);\
    }while(0);\

int main()
{
    int server_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if(server_fd < 0)
        errExit("socket");

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(atoi("8888"));
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char buff[BUFFSIZE] = {0};
    while(1)
    {
        bzero(buff, sizeof(buff));
        scanf("%s", buff);

        int nsend = sendto(server_fd, buff, strlen(buff) + 1, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        if(nsend < 0)
            errExit("send");

        bzero(buff, sizeof(buff));
        int nread = recvfrom(server_fd, buff, nsend, 0, NULL, NULL);
        if(nread < 0)
            errExit("recvfrom");
        fprintf(stdout, "%s\n", buff);
    }
    close(server_fd);
    return 0;
}
