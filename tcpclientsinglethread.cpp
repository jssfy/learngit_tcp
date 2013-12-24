#include <assert.h>
#include <stdio.h>
#include "tools.h"
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

struct timeval gStart;
const int SIZE = 20000000;
#define DEFAULT_PORT 15800

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("%s: sizeofservers_in_dest destAddress[1+]\n", argv[0]);
		return 0;
	}
	int sizeofservers;
	if (argc > 1) {
		assert(argv[1] > 0);
		sizeofservers = atoi(argv[1]);
	}
	// added to read from file
	char *input = NULL;
	unsigned int lLen;
	char time_buf[30];
	GetLocalTime(&time_buf[0]);
	printf("pid:%u, %s start to read\n", getpid(), time_buf);
	char lpFile[] = "input.jpg";
	ReadFileToSingleBuffer(&lpFile[0], &input, lLen);
	if (lLen < 0) {
		perror("file empty or not existing");
		exit(1);
	}
	GetLocalTime(&time_buf[0]);
	printf("pid:%u, %s read ready, to send\n", getpid(), time_buf);

	struct hostent he[1000], *temp;
	//he = new struct hostent[argc];
	for (int i = 2; i < argc ; i++) {
		temp = gethostbyname(argv[i]);
		he[i] = *temp;
	}
	for (int i = 2; i < argc ; i++) {
		printf("server name: %s\n", *he[i].h_aliases);
	}

	struct timeval start, mid, end;
	struct timezone tz;
	gettimeofday(&start, &tz);

	struct sockaddr_in their_addr;
	int socks[1000];	// assume there are 1000 servers at most
	for (int i = 2; i < argc ; i++) {
		for (int k = 0; k < sizeofservers; k++) {
			//printf ("%d\n", argc);
			// to spend time once more to updat this hostent
			temp = gethostbyname(argv[i]);
			he[i] = *temp;
			int sockfd, numbytes;
			char *buf;
			buf = new char[SIZE];
			if (NULL == buf) {
				perror("new");
				return 0;
			}

			if ((socks[(i-1)*sizeofservers+k] = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			//if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
				perror("socket");
				exit(1);
			}
			their_addr.sin_family = AF_INET;
			their_addr.sin_port = htons(DEFAULT_PORT + i - 1);
			their_addr.sin_addr = *((struct in_addr *)he[i].h_addr);
			bzero(&(their_addr.sin_zero), 8);

			sockfd = socks[(i-1)*sizeofservers+k];
			if (connect
			    (sockfd, (struct sockaddr *)&their_addr,
			     sizeof(struct sockaddr)) == -1) {
				perror("connect");
				exit(1);
			}
			GetLocalTime(&time_buf[0]);
			printf("pid:%u, %s connect done, to send\n",
			       getpid(), time_buf);
			}
		}

	gettimeofday(&mid, &tz);
	int sockfd;
	for (int i = 2; i < argc ; i++) {
		for (int k = 0; k < sizeofservers; k++) {
			int j = 1;
			while (j-- > 0) {
				// int len = SIZE;
				int lSent = 0;
				sockfd = socks[(i-1)*sizeofservers+k];
				if ((lSent =
				     send(sockfd, &lLen, sizeof(int),
				     //send(socks[(i-1)*sizeofservers+k], &lLen, sizeof(int),
					  0)) == -1) {
					perror("send");
					exit(1);
				}
				GetLocalTime(&time_buf[0]);
				P(("pid:%u, %s, to send content\n",
				       getpid(), time_buf));
				unsigned lToSend = lLen, lIndex = 0;
				while (lToSend > 0) {
					if (lIndex > 0)
						printf
						    ("sent more than once \n");
					if ((lSent =
					     send(sockfd, input, lLen,
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
				P(("pid:%u, %s sending finished\n",
				       getpid(), time_buf));
				input -= lIndex;

			}
			shutdown(sockfd, 2);
			//if(buf != NULL )delete [] buf;
			close(sockfd);
		}
	}
	if (input != NULL)
		delete[]input;

	gettimeofday(&end, &tz);
	printf("time consumed (include connect): %ld us\n",
	       end.tv_usec - start.tv_usec + (end.tv_sec -
					      start.tv_sec) * 1000000);
	printf("time consumed (after connect): %ld us\n",
	       end.tv_usec - mid.tv_usec + (end.tv_sec -
					      mid.tv_sec) * 1000000);
	//delete[]he;
}
