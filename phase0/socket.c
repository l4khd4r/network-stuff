#define _POSIX_C_SOURCE 200112L /* this flag remove the syntax errors if u using the shitty vscode*/
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(void)
{
	int status;
	struct addrinfo hint;
	struct addrinfo *res = NULL;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_PASSIVE;
	/*
	getaddrinfo(NULL, "3890", AI_PASSIVE)
			    ↓
			returns 0.0.0.0:3890

			socket()
			    ↓
			creates FD

			bind()
			    ↓
			attaches socket to 0.0.0.0:3890

			listen()
			    ↓
			socket becomes LISTEN
	*/

	status = getaddrinfo(NULL, "3490", &hint, &res);
	//By using the AI_PASSIVE flag, I’m telling the program to bind to the IP of the host it’s running on. If you want to bind to a specific local IP address, drop the AI_PASSIVE and put an IP address in for the first argument to getaddrinfo()

	
	if (status != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 1;
	}

	char ipstr[INET_ADDRSTRLEN];

	struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
	if (inet_ntop(AF_INET, &(ipv4->sin_addr), ipstr, sizeof(ipstr)) == NULL) {
		perror("inet_ntop");
		freeaddrinfo(res);
		return 1;
	}

	

	printf("status = %d\n", status);
	printf("IPv4 address = %s\n", ipstr);
	int s = socket(res->ai_family , res->ai_socktype  , res->ai_protocol);
	printf("socket  = %d\n", s );
	
	freeaddrinfo(res);
	close(s);
	return 0;
}