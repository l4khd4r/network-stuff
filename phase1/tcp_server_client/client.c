#define _POSIX_C_SOURCE 200112L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "1337"


#define MAXDATASIZE 1000


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    return &(((struct sockaddr_in*)sa)->sin_addr);

}


int main(int ac , char **av)
{
    int sockfd , numbytes; 
    struct addrinfo hint , *p , *res ;
    char s[INET_ADDRSTRLEN];    
    int rv ;
    memset(&hint , 0 , sizeof(hint));
    hint.ai_family  = AF_INET ;
    hint.ai_socktype = SOCK_STREAM ;
    if(ac != 2)
    {
        printf("usage : ./server [server's hostname]\n");
        return 1;
    }
    if((rv = getaddrinfo(av[1] , PORT , &hint , &res)) != 0)
    {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
                return 1;
    }


    for(p = res ; p != NULL ;  p = p->ai_next)
    {
        if((sockfd = socket(p->ai_family , p->ai_socktype , p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue ;
        }
        inet_ntop(AF_INET , get_in_addr((struct sockaddr *)p->ai_addr) , s  ,sizeof s);
        printf("client: attempting connection to %s\n", s);
        if(connect(sockfd  , p->ai_addr , p->ai_addrlen) ==  -1)
        {
            perror("connect");
            continue ;
        }
        break;
    }
    if(p == NULL )
    {
        fprintf(stderr , "client : failed to connect\n");
        return 1;
    }
    char buf[4092];
    inet_ntop(AF_INET , get_in_addr((struct sockaddr *)p->ai_addr) , s , sizeof s);
    if((numbytes = recv(sockfd   , buf , sizeof buf   , 0)) == -1)
    {
        perror("recv");
        return 1;
    }
    buf[numbytes] = '\n';
    printf("client received : %s\n" , buf);
    close(sockfd);
    return 0;
}