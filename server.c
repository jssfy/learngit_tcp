/*	Server.c 	*/

#include "share.h"

main()
{

	struct sockaddr_in serv_addr, cli_addr;
	socklen_t addr_size = sizeof(cli_addr);
	struct dataUnit mesg;
	struct stat statbuf;
	//struct sockaddr * temptr;
	struct sockaddr temptr;
	struct in_addr *ptr;
	struct hostent *hp;
	int sockfd, n, temp;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("server: can not open datagram socket\n");
		exit(1);
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons((u_short) SERV_UDP_PORT);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("server: can not bind local address");
		exit(1);
	}

	for (;;) {
		//      temptr = (struct sockaddr *) &cli_addr;
		n = recvfrom(sockfd, &mesg, sizeof(mesg), 0, &temptr, &temp);

		if (n < 0) {
			printf("recvfrom error in server.c\n");
			exit(1);
		}

		switch (mesg.type) {
		case 0:
			{
				mesg.type = ADDR_ACK;
				if ((hp = gethostbyname(mesg.str)) == 0) {
					strcat(mesg.str,
					       " does not exit on the internet");
					break;
				}
				ptr = (struct in_addr *)*hp->h_addr_list;
				strcat(mesg.str,
				       " has an internet address which is : ");
				strcat(mesg.str, inet_ntoa(*ptr));
				break;
			}
		case 2:
			{
				mesg.type = FILESIZE_ACK;
				if (stat(mesg.str, &statbuf) < 0)
					strcat(mesg.str,
					       "   Not Found In Our DataBase\n");
				else
					mesg.data = statbuf.st_size;
				break;
			}
		default:
			printf("unkonw mode\n");
			break;
		}		// end of switch

		//      if (sendto(sockfd, &mesg, sizeof(mesg), 0, (struct sockaddr *) &cli_addr, sizeof(cli_addr)) !=n){
		if (sendto
		    (sockfd, &mesg, sizeof(mesg), 0, &temptr,
		     sizeof(cli_addr)) != n) {
			// printf("server: sendto error\n");
			// exit(1);
		}
	}			// end of for
	exit(0);
}				//end of main
