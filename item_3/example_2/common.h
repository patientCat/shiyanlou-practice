#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define SERVER_PORT 8888
#define SERVER_HOST "10.175.142.204"
#define BUFFSIZE 512
#define Perror(val, check_val, note) {\
    if(val < check_val)\
    {   printf(note); exit(-1); }\
}
