#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(void)
{
    int status;
    int socketFd;
    struct addrinfo hint;
    struct addrinfo *res, *p;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;

    status = getaddrinfo("google.com", "http", &hint, &res);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    for (p = res; p != NULL; p = p->ai_next)
    {
        char ipstr[INET_ADDRSTRLEN];
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;

        inet_ntop(AF_INET, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
        printf("Trying IP: %s\n", ipstr);

        socketFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socketFd == -1)
        {
            perror("socket");
            continue;
        }

        if (connect(socketFd, p->ai_addr, p->ai_addrlen) == 0)
        {
            printf("Connected to %s\n", ipstr);
            break;
        }

        perror("connect");
        close(socketFd);
    }

    const char *request = "GET / HTTP/1.1\r\nHost: google.com\r\n\r\n";
    send(socketFd , request , strlen(request) , 0);
    char buffer[4096];

    int bytes ;
    while((bytes = recv(socketFd, buffer ,sizeof(buffer )  -1  ,0)) > 1)
    {
        buffer[bytes ] = '\0';
        printf("%s\n", buffer );
    }
    freeaddrinfo(res);
    return 0;
}