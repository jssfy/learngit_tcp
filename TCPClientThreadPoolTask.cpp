#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include "ThreadManager.h"

extern char *gInput;
extern unsigned int gLen;

namespace KIDD {

	/*int
	   main (int argc, char *argv[])
	   {
	   if (argc != 2)
	   {
	   printf ("%s: IPAddress\n", argv[0]);
	   return 0;
	   } */
	void TCPClientThreadPoolTask::Run() {
		char *lServer = NULL;
		short sockfd = -1;
		char time_buf[30];
		char *input = gInput;
	struct timeval start, end;
	struct timezone tz;
		while ((sockfd = PopConnectionQ()) != -1) {
			// <deleted, used to create socket and connect to server>
			gettimeofday(&start, &tz);
			int j = 1;
			while (j-- > 0) {
				int lSent = 0;

				if ((lSent =
				     send(sockfd, &gLen, sizeof(int),
					  0)) == -1) {
					perror("send");
					exit(1);
				}
				GetLocalTime(&time_buf[0]);
			P(("tid:%lu, %s read ready, to send content\n",
				     pthread_self(), time_buf));
				unsigned lToSend = gLen, lIndex = 0;
				while (lToSend > 0) {
					if (lIndex > 0)
						printf
						    ("sent more than once \n");
					if ((lSent =
					     send(sockfd, input, lToSend,
						  0)) == -1) {
						perror("send");
						break;
					}
					if (lSent > 0) {
						lIndex += lSent;
						lToSend -= lSent;
						input += lSent;
					}
				}
				GetLocalTime(&time_buf[0]);
				gettimeofday(&end, &tz);
				P(("tid:%lu, %s sending finished, consumed %ldus\n",
				       pthread_self(), time_buf,
	       end.tv_usec - start.tv_usec + (end.tv_sec -
					      start.tv_sec) * 1000000));
				input -= lIndex;
			}
			// if (input != NULL)
			//   delete input;
			shutdown(sockfd, 2);
			close(sockfd);
			// if(buf != NULL)delete [] buf;
			//return;
		}
	}
	int ConnectToServer(char*ipServer, short iPort)
	{
			struct timeval start, end;
			gettimeofday(&start, NULL);
			int sockfd, numbytes;
			struct hostent *he;
			struct sockaddr_in their_addr;
			int i = 0;
			he = gethostbyname(ipServer);
			if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
			{
				perror("ConnectToServer->socket");
				exit(1);
			}
			their_addr.sin_port = htons(iPort);
			their_addr.sin_family = AF_INET;
			their_addr.sin_addr = *((struct in_addr *)he->h_addr);
			bzero(&(their_addr.sin_zero), 8);

			char time_buf[30];
			//char *input = new char[lLen];
			//if (NULL == input) {
			//	perror("ConnectToServer->memory");
			//	exit(1);
			//}
			//memcpy(input, lInput, lLen);
			//input = lInput;

			if (connect
			    (sockfd, (struct sockaddr *)&their_addr,
			     sizeof(struct sockaddr)) == -1) {
				perror("ConnectToServer->connect");
				exit(1);
			}
			gettimeofday(&end, NULL);
			GetLocalTime(&time_buf[0]);
			P(("tid:%lu, connected at %s, consumed %ldus\n", pthread_self(),
			       time_buf, end.tv_usec-start.tv_usec+(end.tv_sec-start.tv_sec)*1000000));
			return sockfd;
	}
}
