#include "ThreadManager.h"
#include "tools.h"
#include <assert.h>
#include <stdlib.h>

using namespace KIDD;
#include <stdio.h>
#include <sys/time.h>

char *gInput = NULL;
unsigned int gLen = 0;

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("%s: sizeofservers_in_dest destAddress[1+]\n", argv[0]);
		return 0;
	}
	int sizeofservers;	
	if(argc > 1){
		assert(argv[1] > 0);
		sizeofservers = atoi(argv[1]);
	}

	// added to read from file
	char time_buf[30];
	GetLocalTime(&time_buf[0]);
	P(("tid:%lu, %s start to read\n", pthread_self(), time_buf));
	char lFile[] = "input.jpg";
	ReadFileToSingleBuffer(&lFile[0], &gInput, gLen);
	if (gLen < 0) {
		perror("file empty or not existing");
		return 0;
	}
	GetLocalTime(&time_buf[0]);
	P(("tid:%lu, %s read ready\n", pthread_self(), time_buf));

	//  WorkerThread *lThread = new WorkerThread(new TCPClientTask(argv[1]));
	//  lThread->Start();
	//  lThread->ThreadJoin();

	struct timeval start, mid, end;
	struct timezone tz;
	gettimeofday(&start, &tz);

#define N 8 
	WorkerThread *threads[N];
	InitGlobals();
	for (int i = 0; i < sizeofservers; i++) {
		for (int j = 2; j < argc; j++) {
			int sockfd = ConnectToServer(argv[j], DEFAULT_PORT+i);
			PushConnectionQ(sockfd);
			
		// this Q is not available when the main thread comes out of scope
		//PushServerQ(argv[i]);
		//      threads[i] = new WorkerThread(new TCPClientTask(argv[i]));
		//      threads[i]->Start();
		//      threads[i] = new WorkerThread(new TCPClientTask("10.27.31.62"));
		//      threads[i]->Start();
		//      threads[i] = new WorkerThread(new TCPClientTask("10.27.31.63"));
		//      threads[i]->Start();
		//      threads[i] = new WorkerThread(new TCPClientTask("10.27.31.64"));
		//      threads[i]->Start();
		//threads[i]->ThreadJoin();
		}
	}
	gettimeofday(&mid, &tz);
	for (int i = 1; i <= argc; i++) {
		// or you may use empty() to test the exit condition
		threads[i] = new WorkerThread(new TCPClientThreadPoolTask());
		threads[i]->Start();
	}
	for (int i = 1; i <= argc; i++) {
		// or you may use empty() to test the exit condition
		//printf("%s\n", PopServerQ());
		threads[i]->ThreadJoin();
		delete threads[i];
	}
	gettimeofday(&end, &tz);
	printf("time consumed (including memory copying): %ld us\n",
	       end.tv_usec - start.tv_usec + (end.tv_sec -
					      start.tv_sec) * 1000000);
	printf("time consumed for sending only since thread creation: %ld us\n",
	       end.tv_usec - mid.tv_usec + (end.tv_sec -
					      mid.tv_sec) * 1000000);
	if(gInput != NULL)
	{
		delete[]gInput;
		gInput = NULL;
	}

	return 0;
}
