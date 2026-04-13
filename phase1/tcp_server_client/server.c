#define _POSIX_C_SOURCE 200112L

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

#define BACKLOG 10
#define PORT "1337"

void sigchld_handler(int s)
{
    int saved_errno = errno;
    (void)s;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    errno = saved_errno;
}

int main(void)
{
    int status;
    struct addrinfo hint;
    struct addrinfo *res, *p;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    int server_socket, new_fd;
    int yes = 1;

    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, PORT, &hint, &res);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    for (p = res; p != NULL; p = p->ai_next)
    {
        server_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (server_socket == -1)
        {
            perror("socket");
            continue;
        }

        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
        {
            perror("setsockopt");
            close(server_socket);
            freeaddrinfo(res);
            return 1;
        }

        if (bind(server_socket, p->ai_addr, p->ai_addrlen) == -1)
        {
            perror("server: bind");
            close(server_socket);
            continue;
        }

        break;
    }

    freeaddrinfo(res);

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        return 1;
    }

    if (listen(server_socket, BACKLOG) == -1)
    {
        perror("server: listen");
        close(server_socket);
        return 1;
    }

    if (signal(SIGCHLD, sigchld_handler) == SIG_ERR)
    {
        perror("server: signal");
        close(server_socket);
        return 1;
    }

    printf("server: waiting for connections on port %s\n", PORT);

    while (1)
    {
        addr_size = sizeof(client_addr);

        new_fd = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }
        char s[INET_ADDRSTRLEN];
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)&client_addr;

        inet_ntop(AF_INET, &(ipv4->sin_addr), s, sizeof(s));

        printf("server: got connection from %s\n", s);

        if (fork() == 0)
        {
            const char *msg = "HELLO, WORLD!";

            close(server_socket);

            if (send(new_fd, msg, strlen(msg), 0) == -1)
            {
                perror("send");
            }

            close(new_fd);
            _exit(0);
        }

        close(new_fd);
    }

    close(server_socket);
    return 0;
}