#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main()
{

// create sockte test
	int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 != sd)
	{
		close(sd);
	}
	else
	{
		perror("socket");
		return -1;
	}
	printf("---------------create socket test end\n");
// gethostent test	
	struct hostent * phe;
	phe = gethostent();
	if(NULL != phe)
	{
		printf("name: %s,\n addrtype: %d,\n length: %d,\n IP Address: %s, %x\n", phe->h_name, phe->h_addrtype, phe->h_length, inet_ntoa(*((struct in_addr*)phe->h_addr)), *(int*)(phe->h_addr));
	}
	char **pptr;
	for(pptr = phe->h_aliases; *pptr != NULL; pptr++)
	{
		printf("alias: %s\n", *pptr);
	}
	switch(phe->h_addrtype)
	{
		case AF_INET:
		case AF_INET6:
			pptr = phe->h_addr_list;
			char str[32];
			for(; *pptr!=NULL;pptr++)
			{
				printf("address, %d, %d: %s\n", (*pptr)[0], (*pptr)[1], inet_ntop(phe->h_addrtype, *pptr, str, sizeof(str)));
			}
			break;
		default:
			printf("error\n");
			break;
	}

	printf("---------------gethostent test end\n");
// getaddrinfo test


// other test below
	return 0;
}
