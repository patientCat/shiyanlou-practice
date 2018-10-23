#include "common.hpp"

void
addfdToEpoll(int epfd, int fd, struct epoll_event *ev)
{
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, ev);
    Perror_less(ret, 0, "epoll_ctl add");
}

void 
delfdToEpoll(int epfd, int fd)
{
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
    Perror_less(ret, 0, "epoll_ctl del");
}
