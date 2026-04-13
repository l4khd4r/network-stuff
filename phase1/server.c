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
#define BACKLOG 10
#define PORT "1337"

void sigchld_handler(int s)
{
    (void)s; // quiet unused variable warning

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

int main(void)
{
    int status ;
    struct addrinfo hint ;
    struct addrinfo *res , *p;
    struct sockaddr_storage client_addr ; // connector's address info 
    socklen_t addr_size  ;
    int server_socket  , new_fd ;
    int yes = 1;
    
    memset(&hint , 0 , sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags= AI_PASSIVE;
    


    status = getaddrinfo(NULL , PORT , &hint , &res);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }
    // loop on all of the address and get 
    for(p = res ; p != NULL ; p = p->ai_next )
    {
        if((server_socket = socket(p->ai_family , p->ai_socktype , p->ai_protocol)) == -1)
        {
            perror("socket");
            continue ;
        }



        if(setsockopt(server_socket , SOL_SOCKET , SO_REUSEADDR , &yes , sizeof(yes)) == -1 )
        {
            perror("setsocketopt");
            close(server_socket);
            exit(1);
        }

        if(bind(server_socket , p->ai_addr , p->ai_addrlen) == -1)
        {
            perror("server: bind");
            close(server_socket);
            continue ;
        }
        break ;
    }

    freeaddrinfo(res);
    if(p == NULL)
    {
        fprintf(stderr , "server : failed to bind\n" );
        exit(1);

    }

    if(listen(server_socket , BACKLOG) == -1)
    {
        perror("server: listen");
        exit(1);
    }
    if(signal(SIGCHLD ,sigchld_handler) == -1)
    {
        perror("server: signal");
        return 1;
    }
    printf("server : waiting for connections from port %s" , PORT);

    while(1)
    {
        addr_size = sizeof client_addr ;
        new_fd = accept(server_socket  ,(struct sockaddr *)&client_addr , addr_size );
        if(new_fd == -1)
        {
            perror("accpet");
            return 1;
        }
        if(!fork())
        {
            close(server_socket);
            if(send(new_fd , "HELLO, WORLD!" ,13, 0) == -1)
            {
                perror("send");
                close(new_fd);
            }
            exit(0);

        }
        close(new_fd);
        
    }

}