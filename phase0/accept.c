#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "1337"
#define BACKLOG 10
#define BUFFER_SIZE 4096

int main(void)
{
    int status;
    int yes = 1;
    int serverFd;
    int clientFd;
    ssize_t bytes;

    struct addrinfo hints;
    struct addrinfo *res;
    struct sockaddr_storage client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    char buffer[BUFFER_SIZE];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;          // IPv4
    hints.ai_socktype = SOCK_STREAM;    // TCP
    hints.ai_flags = AI_PASSIVE;        // 0.0.0.0

    status = getaddrinfo(NULL, PORT, &hints, &res);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    serverFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (serverFd == -1)
    {
        perror("socket");
        freeaddrinfo(res);
        return 1;
    }

    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        perror("setsockopt");
        close(serverFd);
        freeaddrinfo(res);
        return 1;
    }

    if (bind(serverFd, res->ai_addr, res->ai_addrlen) == -1)
    {
        perror("bind");
        close(serverFd);
        freeaddrinfo(res);
        return 1;
    }

    if (listen(serverFd, BACKLOG) == -1)
    {
        perror("listen");
        close(serverFd);
        freeaddrinfo(res);
        return 1;
    }

    printf("Server listening on 0.0.0.0:%s\n", PORT);

    clientFd = accept(serverFd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (clientFd == -1)
    {
        perror("accept");
        close(serverFd);
        freeaddrinfo(res);
        return 1;
    }

    printf("Client connected\n");

    if (send(clientFd, "salut dans mon server de mohamed lakhdar\n", strlen("salut dans mon server de mohamed lakhdar\n"), 0) == -1)
        perror("send");


    bytes = recv(clientFd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes == -1)
    {
        perror("recv");
    }
    else if (bytes == 0)
    {
        printf("Client closed connection\n");
    }
    else
    {
        buffer[bytes] = '\0';
        printf("Received: %s\n", buffer);
    }

    close(clientFd);
    close(serverFd);
    freeaddrinfo(res);
    return 0;
}