#include "client.hpp"

void 
Client::untilConnect()
{
    printf(" Client is initializing\n");
    // 设置相应端口号和ip地址
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = PF_INET; //AF_INET对应于BSD标准, PF_INET对应于Posix标准
    serverAddr.sin_port = htons(SERVER_PORT); // 设置端口号，主机字节序需要转网络序
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP); // 设置ip地址
    // 创建socket
    _sockfd = socket(PF_INET, SOCK_STREAM, 0);
    Perror_less(_sockfd, 0, "socket error");

    // 连接服务端
    int ret = ::connect(_sockfd, (const struct sockaddr*) &serverAddr, sizeof(struct sockaddr));
    Perror_less(ret, 0, "connect error");

    // 创建管道
    ret = pipe(_pipefd); // fd[0] 用于父进程读，fd[1]用于子进程写
    Perror_less(ret, 0, "pipe error");

    _epfd = epoll_create1(0); 

    // 将_sockfd 和_pipefd[0]加入_epfd;
    struct epoll_event ev;
    ev.data.fd = _sockfd;
    ev.events = EPOLLIN;

    bool isNonBlocked = false;
    addfdToEpoll(_epfd, _sockfd, &ev, isNonBlocked);
    ev.data.fd = _pipefd[0];
    addfdToEpoll(_epfd, _pipefd[0], &ev, isNonBlocked);
    printf(" Client has intialized successfully\n");
}

void 
Client::end()
{
    // 关闭所有打开的文件描述符
    close(_sockfd);
    // 需要注意判断是在父进程还是子进程，客户端的俩个进程打开的管道文件描述符不同
    if(_isFatherProcess)
    {
        close(_pipefd[0]);
    }
    else{
        close(_pipefd[1]);
    }
}


void 
Client::start()
{
    untilConnect();

    int ret = fork();
    if(!ret)
    {
        // 进入子进程
        // 关闭子进程的读管道
        close(_pipefd[0]);

        // 子进程负责收集用户输入的消息并写入管道
        int ret;

        while(1)
        {
            memset(_buff, 0, BUFFSIZE);
            ret = read(STDIN_FILENO, _buff, BUFFSIZE - 1);
            if(!ret)
                break;
            write(_pipefd[1], _buff, ret);
        }
    }
    else if(ret > 0)
    {
        // 进入父进程
        // 关闭父进程的写管道
        close(_pipefd[1]);

        struct epoll_event events[2];
        while(1)
        {
            int num = epoll_wait(_epfd, events, 2, -1);
            if(num <= 0)
            {
                printf("epfd_wait error\n");
                exit(-1);
            }
            for(int i = 0; i < num; ++i)
            {
                // 收到来自服务器的消息
                bzero(_buff, BUFFSIZE);
                if(events[i].data.fd == _sockfd)
                {
                    int ret = recv(_sockfd, _buff, BUFFSIZE - 1, 0); // 有疑问
                    write(STDOUT_FILENO, _buff, ret);
                    printf("%s", _buff);
                }
                if(events[i].data.fd == _pipefd[0])
                {
                    int ret = read(_pipefd[0], _buff, BUFFSIZE - 1);
                    send(_sockfd, _buff, ret, 0);
                    printf("%*cI say >> %s", 50, ' ', _buff);
                }
            }
        }
    }
    else{
        printf(" fork error \n");
        exit(-1);
    }
    end();
}
