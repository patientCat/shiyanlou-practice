#ifndef _COMMON_H_
#define _COMMON_H_

#include <sys/types.h>          
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>

#define SERVER_PORT 8888
#define SERVER_IP "192.168.42.3"
#define BUFFSIZE 512
#define MAX_CONNECTED_CLIENTS 10

#define Perror_less(val, check_val, note)    {\
    if(val < check_val) \
    {   printf(note); exit(-1); }\
}

#define Perror_greater(val, check_val, note)    {\
    if(val > check_val) \
    {   printf(note); exit(-1); }\
}
#define Perror_equal(val, check_val, note)    {\
    if(val == check_val) \
    {   printf(note); exit(-1); }\
}

#define SERVER_MESSAGE "client %d: says >> %s"
static void
addfdToEpoll(int epfd, int fd, struct epoll_event *ev, bool isNonBlocked)
{
    if(isNonBlocked)
    {
        fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
    }
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, ev);
    Perror_less(ret, 0, "epoll_ctl add");
}

static void 
delfdToEpoll(int epfd, int fd)
{
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
    Perror_less(ret, 0, "epoll_ctl del");
}

static void 
setNonBlock(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}


#endif /* common_h */

