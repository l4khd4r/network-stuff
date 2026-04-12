#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(void)
{
    int socketFd;
    int yes = 1;
    int status;
    struct addrinfo hint;
    struct addrinfo *res;

    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, "3490", &hint, &res);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    socketFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socketFd == -1)
    {
        perror("socket");
        freeaddrinfo(res);
        return 1;
    }

    if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        perror("setsockopt");
        close(socketFd);
        freeaddrinfo(res);
        return 1;
    }

    if (bind(socketFd, res->ai_addr, res->ai_addrlen) == -1)
    {
        perror("bind");
        close(socketFd);
        freeaddrinfo(res);
        return 1;
    }

    if (listen(socketFd, 20) == -1)
    {
        perror("listen");
        close(socketFd);
        freeaddrinfo(res);
        return 1;
    }

    printf("Server listening on port 3490\n");
    pause();
    freeaddrinfo(res);
    close(socketFd);
    return 0;
}