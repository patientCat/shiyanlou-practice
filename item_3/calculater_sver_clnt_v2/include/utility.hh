#ifndef _UTILITY_HH_
#define _UTILITY_HH_

#include <stdlib.h> 
#include <sys/socket.h> 
#include <stdio.h> 
#include <errno.h>
#include <string.h> 

#define BASIC_SIZE 9
typedef struct _train_t{
    int operandSize;
    int operandTypeSize;
    char *operandArr;
    char operSymbol;
}Train_t;

int sendn(int socket, const void *buffer, size_t length, int flags)
{
    char *buff = (char *)buffer;
    size_t totSended = 0;
    ssize_t nsend = 0;
    while(totSended < length)
    {
        nsend = send(socket, buff, length - totSended, flags); 
        if(nsend <= 0)
        {
            if( nsend == -1 && errno == EINTR)
                continue;
            else
                return -1;
        }
        totSended += nsend;
        buff += nsend;
    }
    return totSended;
}

int recvn(int socket, void *buffer, size_t length, int flags)
{
    char *buff = (char *)buffer;
    size_t totRecved = 0;
    ssize_t nRecved = 0;
    while(totRecved < length)
    {
        nRecved = recv(socket, buff, length - totRecved, flags); 
        if(nRecved <= 0)
        {
            if( !nRecved )
                return totRecved;
            if( nRecved == -1 && errno == EINTR)
                continue;
            else
                return -1;
        }
        totRecved += nRecved;
        buff += nRecved;
    }
    return totRecved;
}

void sendtrain(int fd, Train_t *msg, int flag)
{
    int trainLength = BASIC_SIZE + msg->operandTypeSize * msg->operandSize;
    if(sendn(fd, msg, trainLength, flag) < 0)
    {
        perror("sendn");
        exit(EXIT_FAILURE);
    }
}

void recvtrain(int fd, void *buff, int flag)
{
    int length;
    if(recvn(fd, &length, 4, flag) < 0)
    {
        perror("recvn");
        exit(EXIT_FAILURE);
    }
    if(recvn(fd, buff, length, flag) < 0)
    {
        perror("recvn");
        exit(EXIT_FAILURE);
    }
}

#endif // utility.hh
