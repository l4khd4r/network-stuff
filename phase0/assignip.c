#include <arpa/inet.h>
#include <stdio.h>


int main()
{
	struct sockaddr_in sa ;
	// presentation to network
	inet_pton(AF_INET ,"192.168.1.1"  , &(sa.sin_addr) );
	printf("%d\n", sa.sin_addr.s_addr);
	char ip4[INET_ADDRSTRLEN];
	
	// network to presentation
	/*
	
	struct in_addr {
		uint32_t s_addr; // that's a 32-bit int (4 bytes)
	};

	*/
	inet_ntop(AF_INET , &(sa.sin_addr) , ip4 , INET_ADDRSTRLEN);
	printf("%s\n",ip4);


	struct sockaddr_in6 sa6 ;
	// presentation to network
	inet_pton(AF_INET6 ,"2001:db8:63b3:1::3490"  , &(sa6.sin6_addr) );
	printf("%d\n", sa6.sin6_addr.__in6_u);
	char ip6[INET6_ADDRSTRLEN];
	
	// network to presentation
	
	inet_ntop(AF_INET6, &(sa6.sin6_addr) , ip6 , INET6_ADDRSTRLEN);
	printf("%s\n",ip6);

} 