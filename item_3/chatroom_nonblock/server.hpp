#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include "common.hpp"

#include <iostream>
#include <algorithm>

#include <vector>

using std::vector;

class Server
{
public: 
    Server()
    : _isRunning(false)
    {}
    void init();
    void start();
    void end();
private:
    int sendBroadcastMessage(int);
    void processDisconnect(int);
    void processNewConnection();

private:
    int _listenfd;
    int _epfd;
    vector<int> _clientsfd;
    bool _isRunning;
};
#endif /* server_hpp */
