//c++ program
//design by >^.^<
//@date    2018-11-17 20:47:37

// server

#include "head.hh" 

#define errExit(msg) \
    do{\
        fprintf(stderr, "%s\n", msg);\
        exit(-1);\
    }while(0);\

int main()
{
    int client_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if(client_fd < 0)
        errExit("socket");

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi("8888"));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = bind(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));    
    if(ret < 0)
        errExit("bind");

    // no listen , no accept
    struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(client_addr));
    socklen_t socklen;
    char buff[BUFFSIZE] = {0};
    while(1)
    {
        bzero(buff, sizeof(buff));
        int nread = recvfrom(client_fd, buff, BUFFSIZE-1, 0, (struct sockaddr*)&client_addr, &socklen);
        printf("nread = %d\n", nread);
        if(nread < 0)
            errExit("recvfrom");
        int nsend = sendto(client_fd, buff, nread, 0, (struct sockaddr*)&client_addr, socklen);
        if(nsend < 0)
            errExit("send");
    }
    close(client_fd);
    return 0;
}
