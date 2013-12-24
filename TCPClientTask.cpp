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
extern unsigned int lLen;
#define DEFAULT_PORT 15800

#define SLEEP_TIME 60*5

namespace KIDD {

	TCPClientTask::
	    TCPClientTask(const char *iServerName):mServerName(iServerName) {
			mPort = DEFAULT_PORT;
			mSockfd = 0;
	} 
	
	TCPClientTask::
	    TCPClientTask(const char *iServerName, short iPort):
				mServerName(iServerName),
				mPort(iPort){
				mSockfd = 0;
	} 

	TCPClientTask::~TCPClientTask()
	{
		if(mInput != NULL)
		{
			delete [] mInput;
		}
	}
	
	void TCPClientTask::Init2() {
		struct timeval start, end;
		struct timeval sc, mid;
		struct timezone tz;
		struct timeval gStart;
		gettimeofday(&gStart, &tz);
		he = gethostbyname(mServerName.c_str());
		gettimeofday(&start, &tz);
		printf("tid:%lu gethostbyname, %ld us\n", pthread_self(),
		       start.tv_usec - gStart.tv_usec + (start.tv_sec -
							 gStart.tv_sec) *
		       1000000);
		
		// to connect in advance
		int numbytes;
		struct sockaddr_in their_addr;
		int i = 0;
		char sc_buf[30], time_buf[30];
		//he = gethostbyname (argv[1]);
		if ((mSockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("socket");
			exit(1);
		}
		gettimeofday(&sc, &tz);
		GetLocalTime(&sc_buf[0]);
		printf("tid:%lu socket created at %s, %ld us\n", pthread_self(), sc_buf,
		       sc.tv_usec - gStart.tv_usec + (sc.tv_sec -
							 gStart.tv_sec) *
		       1000000);
		their_addr.sin_family = AF_INET;
		their_addr.sin_port = htons(mPort);
		their_addr.sin_addr = *((struct in_addr *)he->h_addr);
		bzero(&(their_addr.sin_zero), 8);

		// added to read from file
		//  char *input = NULL;
		//  unsigned int lLen = 0;

		//GetLocalTime(&time_buf[0]);
		// not necessary to copy, there is no modification
		// and advancing in mInput will not modify lInput
		//char *mInput = new char[lLen];
		//if (NULL == mInput) {
		//	perror("memory");
		//	exit(1);
		//}
		mInput = gInput;
		gettimeofday(&sc, &tz);
		//memcpy(mInput, lInput, lLen);
		gettimeofday(&mid, &tz);

		GetLocalTime(&time_buf[0]);
		P(("tid:%lu, %s before connect, %ld us\n", pthread_self(),
		       time_buf,
		       mid.tv_usec - sc.tv_usec + (mid.tv_sec -
							 sc.tv_sec) *
		       1000000));
		// to re-obtain the start time for connect
		gettimeofday(&mid, &tz);
		if (connect
		    (mSockfd, (struct sockaddr *)&their_addr,
		     sizeof(struct sockaddr)) == -1) {
			perror("connect");
			exit(1);
		}
		

		GetLocalTime(&time_buf[0]);
		gettimeofday(&end, &tz);
		P(("tid:%lu, %s connecting finished, %ld us\n",
		       pthread_self(), time_buf,
		       end.tv_usec - mid.tv_usec + (end.tv_sec -
						      mid.tv_sec) * 1000000));

#ifdef JUNHUA_DEBUG
		// support for Junhua's test
		int mSleep = SLEEP_TIME;
		printf("to sleep %d(s) after connect\n", mSleep);
		sleep(mSleep);
		printf("sleep is over\n");
#endif
	}
	void TCPClientTask::Run() {

	// sc, socket creation
		struct timeval sc, start,mid, end;
		struct timeval si, con; //socket initilization, connect
		struct timezone tz;
		char time_buf[30];
		char *input = mInput;

		struct timeval gStart;
		gettimeofday(&gStart, &tz);

		int sockfd, numbytes;

		if(mSockfd == 0){
		struct sockaddr_in their_addr;
		int i = 0;
		char sc_buf[30], time_buf[30];
		//he = gethostbyname (argv[1]);
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("socket");
			exit(1);
		}
		gettimeofday(&sc, &tz);
		GetLocalTime(&sc_buf[0]);
		P(("tid:%lu socket created at %s, %ld us\n", pthread_self(), sc_buf,
		       sc.tv_usec - gStart.tv_usec + (sc.tv_sec -
							 gStart.tv_sec) *
		       1000000));
		their_addr.sin_family = AF_INET;
		their_addr.sin_port = htons(mPort);
		their_addr.sin_addr = *((struct in_addr *)he->h_addr);
		bzero(&(their_addr.sin_zero), 8);

		// added to read from file
		//  char *input = NULL;
		//  unsigned int lLen = 0;

		//GetLocalTime(&time_buf[0]);
		//char *input = new char[lLen];
		//if (NULL == input) {
		//	perror("memory");
		//	exit(1);
		//}
		//input = lInput;
		GetLocalTime(&time_buf[0]);


		gettimeofday(&mid, &tz);
		P(("tid:%lu, %s before connect, %ld us\n", pthread_self(),
		       time_buf,
		       mid.tv_usec - sc.tv_usec + (mid.tv_sec -
							 sc.tv_sec) *
		       1000000));
		// to re-obtain the start time for connect
		gettimeofday(&mid, &tz);
		if (connect
		    (sockfd, (struct sockaddr *)&their_addr,
		     sizeof(struct sockaddr)) == -1) {
			perror("connect");
			exit(1);
		}
		

		GetLocalTime(&time_buf[0]);
		gettimeofday(&end, &tz);
		P(("tid:%lu, %s connecting finished, %ld us\n",
		       pthread_self(), time_buf,
		       end.tv_usec - mid.tv_usec + (end.tv_sec -
						      mid.tv_sec) * 1000000));

#ifdef JUNHUA_DEBUG
		// support for Junhua's test
		int mSleep = SLEEP_TIME;
		printf("to sleep %d(s) after connect\n", mSleep);
		sleep(mSleep);
		printf("sleep is over\n");
#endif

}
else 
{
	sockfd = mSockfd;
}
		gettimeofday(&con, &tz);
		int j = 1;
		while (j-- > 0) {
			///// int len = SIZE;
			int lSent = 0;

			if ((lSent = send(sockfd, &lLen, sizeof(int), 0)) == -1) {
				perror("send");
				exit(1);
			}
			GetLocalTime(&time_buf[0]);
			gettimeofday(&start, &tz);
		P(("tid:%lu, %s read len ready, to send content %ld us\n",
			     pthread_self(), time_buf,
			     start.tv_usec - con.tv_usec + (start.tv_sec -
							    con.tv_sec) *
			     1000000));
			unsigned lToSend = lLen, lIndex = 0;
			while (lToSend > 0) {
				if (lIndex > 0)
					printf("sent more than once \n");
				if ((lSent =
				     send(sockfd, input, lToSend, 0)) == -1) {
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
			P(("tid:%lu, %s sending finished, %ld us\n",
			       pthread_self(), time_buf,
			       end.tv_usec - con.tv_usec + (end.tv_sec -
							      con.tv_sec) *
			       1000000));
			input -= lIndex;
		}
		// if (input != NULL)
		//   delete input;
		shutdown(sockfd, 2);
		close(sockfd);
		// if(buf != NULL)delete [] buf;
		return;
	}
}
