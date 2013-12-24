#include "share.h"
#include "ThreadManager.h"

namespace KIDD {
	void UDPClientTask::Run() {
		int sockfd, n;
		struct sockaddr_in serv_addr;
		struct hostent *hp;
		struct dataUnit mesg;

		if ((hp = gethostbyname(mServerName.c_str())) == 0) {
			printf("gethostbyname error in client '%s'\n",
			       mServerName.c_str());
			exit(1);
		}

		bzero((char *)&serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		memcpy((void *)&serv_addr.sin_addr, hp->h_addr, hp->h_length);
		serv_addr.sin_port = htons((u_short) SERV_UDP_PORT);

		if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			printf("client: can not open datagram socket\n");
			exit(1);
		}

		mesg.data = 0;
		if (strcmp(mMesgType.c_str(), "filesize_req") == 0)
			mesg.type = 2;
		else if (strcmp(mMesgType.c_str(), "addr_req") == 0)
			mesg.type = 0;
		else {
			printf
			    ("Please type filesize_req or addr_req exactly\n");
			exit(1);
		}
		strcpy(mesg.str, mMesg.c_str());
		memset(mesg.str, '1', sizeof(mesg.str) - 1);
		int ret =
		    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF,
			       (const char *)sizeof(mesg.str), sizeof(int));
		if (ret < 0)
			perror("setsockopt");

		n = sendto(sockfd, &mesg, sizeof(mesg), 0,
			   (struct sockaddr *)&serv_addr, sizeof(serv_addr));
		if (n < 0) {
			printf("sendto error in client; the n is %d\n", n);
			perror("sendto");
			exit(1);
		}

		if (recvfrom
		    (sockfd, &mesg, sizeof(mesg), 0, (struct sockaddr *)0,
		     (socklen_t *) 0) < 0) {
			printf("recvfrom error in client\n");
			exit(1);
		}

		printf("====================================\n");
		if (mesg.type == 3) {
			printf("FILESIZE_ACK\n");
			printf("%ld\n", mesg.data);
			printf("%s\n", mesg.str);
			printf("====================================\n");
		}

		if (mesg.type == 1) {
			printf("ADDR_ACK\n");
			printf("%s\n", mesg.str);
			printf("====================================\n");
		}
		exit(0);
	}

}
