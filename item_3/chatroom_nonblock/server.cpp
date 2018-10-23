#include "server.hpp"

void 
Server::init()
{
    printf(" Server is initializing\n");
    // 设置相应端口号和ip地址
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = PF_INET; //AF_INET对应于BSD标准, PF_INET对应于Posix标准
    serverAddr.sin_port = htons(SERVER_PORT); // 设置端口号，主机字节序需要转网络序
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP); // 设置ip地址

    // 创建监听socket
    _listenfd = socket(PF_INET, SOCK_STREAM, 0);
    Perror_less(_listenfd, 0, "socket error");

    // 绑定地址
    int ret = bind(_listenfd, (struct sockaddr*) &serverAddr, sizeof(struct sockaddr));
    Perror_less(ret, 0, "bind error");

    // 监听连接
    ret = listen(_listenfd, 5);
    Perror_less(ret, 0, "listen error");

    // 创建_epfd
    _epfd = epoll_create(10);

    // 让fd和事件关联
    int enable_ET = 1;
    struct epoll_event ev;
    ev.data.fd = _listenfd;
    ev.events = EPOLLIN;
    if(enable_ET)
        ev.events |= EPOLLET;
    // 添加监听事件
    bool isNonBlocked = true;
    addfdToEpoll(_epfd, _listenfd, &ev, isNonBlocked);
    printf(" Server has initialized successfully\n");
}

void
Server::end()
{
    close(_listenfd);
    close(_epfd);
    for(int i = 0; i < (int)_clientsfd.size(); ++i)
        close(_clientsfd[i]);
}

int
Server::sendBroadcastMessage(int clientfd) // 发送广播消息给所有客户端
{
    // 从clientfd处接收信息，然后广播给除clientfd之外的所有人
    char buff[BUFFSIZE] = {0};
    char message[BUFFSIZE] = {0};
    int ret = 0;
    int pos = 0;
    do
    {
        ret = recv(clientfd, buff + pos, BUFFSIZE - 1, 0);
        pos += ret;
    }while(errno != EAGAIN ); 

    // 格式化处理信息
    sprintf(message, SERVER_MESSAGE, clientfd, buff);
    printf("test: %s\n", message);
    for(int i = 0; i < (int)_clientsfd.size(); ++i)
    {
        if(_clientsfd[i] != clientfd)
        {
            send(_clientsfd[i], message, strlen(message), 0);
        }
    }
}

// 用来处理有客户端断开连接的情况
void 
Server::processDisconnect(int disconnfd)
{
    // 将disconnfd从已经建立连接队列中删除
    _clientsfd.erase(std::find(_clientsfd.begin(), _clientsfd.end(), disconnfd));
    // 从epoll监听集中删除
    delfdToEpoll(_epfd, disconnfd);
}

void 
Server::processNewConnection()
{
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    socklen_t len = sizeof(addr);
    int newClientfd = accept(_listenfd, (struct sockaddr *)&addr, &len);
    Perror_less(newClientfd, 0, "accept error");

    printf("client connection from %s:%d:clientId:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), newClientfd);
    char message[BUFFSIZE] = {0};
    sprintf(message, "Your clientId is %d\n", newClientfd);
    send(newClientfd, message, strlen(message), 0);

    struct epoll_event ev;
    ev.data.fd = newClientfd;
    ev.events = EPOLLIN;
    bool isNonBlocked = true;
    addfdToEpoll(_epfd, newClientfd, &ev, isNonBlocked);
    _clientsfd.push_back(newClientfd);
}
void 
Server::start()
{
    init();
    if(!_isRunning)
        _isRunning = true;
    else{
        printf("The server has been running\n");
        return;
    }
    struct epoll_event events[MAX_CONNECTED_CLIENTS + 1]; // _listenfd 要占掉一个
    memset(events, 0, sizeof(events));
    while(_isRunning)
    {
        int num = epoll_wait(_epfd, events, MAX_CONNECTED_CLIENTS + 1, -1);
        Perror_less(num, 0, "epoll_wait");
        for(int i = 0; i < num; ++i)
        {
            // 如果是新连接，则添加到_epfd 中
            if(events[i].data.fd == _listenfd)
            {
                // 这里设置了最大连接数
                if(_clientsfd.size() < MAX_CONNECTED_CLIENTS)
                {
                    processNewConnection();
                }
                else{// 超出最大连接数时，不再接受新的连接
                    printf("There has been %d connection\n", MAX_CONNECTED_CLIENTS);
                }
                continue;
            }
            // 如果是新消息，则广播给其他客户端
            char buff[BUFFSIZE] = {0};
            // 首先找到是哪个客户端，找到了就结束循环
            if(events[i].events & EPOLLIN)
            {
                // PEEK下收到的是什么消息
                int ret = recv(events[i].data.fd, buff, BUFFSIZE - 1, MSG_PEEK);
                if(!ret) // 如果是客户端断开连接的消息
                {
                    processDisconnect(events[i].data.fd);
                    printf("one of clients has disconnected!\n");
                }
                else{
                    sendBroadcastMessage(events[i].data.fd);
                }
            }
        }
    }// end while(1)
    end();
}
