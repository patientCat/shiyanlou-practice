#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include "common.hpp"

class Client
{
public:
    void untilConnect();
    void start();
    void end();

private:
    int _sockfd;
    int _pipefd[2];
    int _epfd;
    bool _isFatherProcess;
    char _buff[BUFFSIZE];
};
#endif /* client_hpp */
