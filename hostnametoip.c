#include<stdio.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
	if(argc != 2) 
	{
		printf("usage: %s hostname\n", argv[0]);
		exit(-1);
	}
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int ret;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
//	hints.ai_socktype = SOCK_STREAM;
	hints.ai_socktype = SOCK_DGRAM;
	
	ret = getaddrinfo(argv[1], NULL, &hints, &result);
	if(ret != 0)
	{
		perror("getaddrinfo");
		exit(-1);
	}

	for(rp = result; rp != NULL; rp = rp->ai_next)
	{
		char hostname[100] = "";
		printf("flags %d, family %d, socktype %d, protocol %d \n", rp->ai_flags, rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		printf("host: %s\n", rp->ai_canonname?rp->ai_canonname:"-");
		if(rp->ai_family == AF_INET)
		{
			struct sockaddr_in *psi = rp->ai_addr;
			char buf[INET_ADDRSTRLEN];
			char *addr = inet_ntop(AF_INET, &psi->sin_addr, buf, INET_ADDRSTRLEN);
			printf("address: %s\n", addr?addr:"unknown");
			printf("port %d\n", ntohs(psi->sin_port));

			ret = getnameinfo(rp->ai_addr, sizeof(struct sockaddr), hostname, 100, NULL, 0, 0); //NI_NUMERICHOST);
			if(ret != 0)
			{
				printf("getnameinfo fail\n");
				gai_strerror(ret);
				continue;
			}
			printf("host name from getnameinfo: %s\n", buf);
		}
		printf("***********\n");
	}
	freeaddrinfo(result);

	return 0;
}
