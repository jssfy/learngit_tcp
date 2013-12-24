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
struct timeval gStart;
const int SIZE = 20000000;
int main(int argc, char *argv[])
{
	gettimeofday(&gStart, NULL);
	if (argc != 2) {
		printf("%s: IPAddress\n", argv[0]);
		return 0;
	}
	int sockfd, numbytes;
	char *buf;
	buf = new char[SIZE];
	if (NULL == buf) {
		perror("new");
		return 0;
	}
	struct hostent *he;
	struct sockaddr_in their_addr;
	int i = 0;
	//??????????
	he = gethostbyname(argv[1]);
	//????TCP???
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	//??????,???????2323??
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(15800);
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);
	//????????
	if (connect
	    (sockfd, (struct sockaddr *)&their_addr,
	     sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
	//?????????
	int j = 1;
	while (j-- > 0) {
		//scanf ("%s", &buf);
		memset(buf, '0' + j, SIZE);
		//buf[sizeof(buf)-1] = 0;
		int len = SIZE;
		int lSent = 0;
		if ((lSent = send(sockfd, &len, sizeof(int), 0)) == -1) {
			perror("send");
			exit(1);
		}
		struct timeval start, end;
		struct timezone tz;
		gettimeofday(&start, &tz);
		if ((lSent = send(sockfd, buf, len, 0)) == -1) {
			perror("send");
			exit(1);
		}
		memset(buf, 0, len);
		printf("%d bytes sent\n", lSent);
		gettimeofday(&end, &tz);
		printf("start %ld, end %ld, time consumed: %ld us\n",
		       start.tv_usec + start.tv_sec * 1000000 -
		       gStart.tv_usec - gStart.tv_sec * 1000000,
		       end.tv_usec + end.tv_sec * 1000000 -
		       gStart.tv_usec - gStart.tv_sec * 1000000,
		       end.tv_usec - start.tv_usec + (end.tv_sec -
						      start.tv_sec) * 1000000);
		//???????????
		//   if ((numbytes = recv (sockfd, buf, 100, 0)) == -1)
		//{
		// perror ("recv");
		//exit (1);
		//}
		// buf[numbytes] = '\0';
		//printf ("result:%s\n", buf);
	}
	shutdown(sockfd, 2);
	close(sockfd);
	return 0;
}
