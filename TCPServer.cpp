#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
//#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
//#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>
#include <stdlib.h>
#include "ThreadManager.h"
using namespace KIDD;

struct timeval gStart;

int main(int argc, char* argv[])
{
	int lNumberOfServers = 1;
	if(argc > 1){
		assert(argv[1] > 0);
		lNumberOfServers = atoi(argv[1]);
	}

	WorkerThread* lpWorkerThreads = new WorkerThread[lNumberOfServers];
	for(int i = 0; i<lNumberOfServers; i++){
		TCPServerTask *lpTask = new TCPServerTask(DEFAULT_PORT + i);
		lpWorkerThreads[i].SetTask(lpTask);

	}

	for(int i = 0; i<lNumberOfServers; i++){
		lpWorkerThreads[i].Start();
	}


	for(int i = 0; i<lNumberOfServers; i++){
		lpWorkerThreads[i].ThreadJoin();

	}

		delete []lpWorkerThreads;

//<deleted for multi-thread support, refer to below commented area for previous code> 
}

// below is the commented code which is previousl used for multi-process test
	/*
	int sockfd, new_fd;
	socklen_t sin_size;
	int numbytes;
	struct sockaddr_in addr, cliaddr;
	socklen_t addr_len;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("createSocket");
		return -1;
	}
	//int lbReuse = 1;
	//setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&lbReuse, sizeof(int));
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(15800);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) ==
	    -1) {
		perror("bind");
		return -1;
	}
	//
	if (listen(sockfd, 10) == -1) {
		perror("listen");
		return -1;
	}
	char *buff;
	//
	while (1) {
		sin_size = sizeof(struct sockaddr_in);
		printf("server is run with pid %d\n", getpid());
		//
		if ((new_fd =
		     accept(sockfd, (struct sockaddr *)&cliaddr,
			    &sin_size)) == -1) {
			perror("accept");
			return -1;
		}
		//
		int pid = fork();
		if (pid < 0) {
			perror("fork");
		} 
		else if (pid == 0) {
			int j = 0;
			printf("0 with pid %u\n", getpid());
			while (j <= 0) {
				int len = -1;
				if ((numbytes =
				     recv(new_fd, &len, sizeof(len), 0)) <= 0) {
							printf("2\n");
					perror("recv");
					printf("pid %u numbytes %d\n", getpid(), numbytes);
					return 0;
				}// if
				P(("%d bytes read, len: %d\n", numbytes, len));
				if (len <= 0) {
					close(new_fd);
					return 0;
				} else{
					buff = new char[len];
				} // if
				if (NULL == buff) {
					close(new_fd);
					return 0;
				}// if
				int lRead = 0;
				char time_buf[30];
				GetLocalTime(&time_buf[0]);
				printf("pid:%u, %s recv start\n", getpid(),
				       time_buf);
				while (len > 0) {
					memset(buff, 0, len);
					// here "buff" will not hold all the data
					if ((numbytes =
					     recv(new_fd, buff, len,
						  0)) == -1) {
							printf("1\n");
						close(new_fd);
						perror("recv");
						return -1;
					} else if (0 == numbytes) {
						printf("connection closed!\n");
						break;
					} // if
					len -= numbytes;
					buff += numbytes;
					lRead += numbytes;
				} // while(len>0)
				GetLocalTime(&time_buf[0]);
				printf("pid:%u, %s recv end, read %u bytes\n",
				       getpid(), time_buf, lRead);
				close(new_fd);
				buff -= lRead;

				// before the end, write to file, and make time stamp
				//GetLocalTime(&time_buf[0]);
				//printf("pid:%u, %s write start\n", getpid(), time_buf);
				if (lRead > 0)
					printf("size of %d read\n", lRead);
					//WriteToFile("output.jpg", buff, lRead);
				GetLocalTime(&time_buf[0]);
				printf("pid:%u, %s write end\n", getpid(),
				       time_buf);

				if (buff != NULL)
					delete[]buff;

				return 0;
			} // while(j<=0)
		} // if(pid<0)
		// close the socket in multi-process mode
		close(new_fd);
	}
	close(sockfd);
	*/
