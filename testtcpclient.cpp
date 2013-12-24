#include <assert.h>
#include <stdlib.h>
#include "ThreadManager.h"
#include "tools.h"

using namespace KIDD;
#include <stdio.h>
#include <sys/time.h>

char *gInput = NULL;
unsigned int lLen = 0;

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
	printf("tid:%lu, %s start to read\n", pthread_self(),
	       time_buf);

	char lpFile[] = "input.jpg";
	ReadFileToSingleBuffer(&lpFile[0], &gInput, lLen);
	if (lLen <= 0) {
		perror("file empty or not existing");
		return 0;
	}
	GetLocalTime(&time_buf[0]);
	printf("tid:%lu, %s read ready\n", pthread_self(),
	       time_buf);

	struct timeval start,mid, end;
	gettimeofday(&start, NULL);
#define N 10
	WorkerThread *threads = new WorkerThread[(argc-2)*sizeofservers];
	for (int i = 0; i < argc-2; i++) {

		for(int j = 0; j<sizeofservers; j++)
		{
			TCPClientTask *lTask = new TCPClientTask(argv[i+2], DEFAULT_PORT+j);
			lTask->Init2();
			//threads[i] = new WorkerThread(lTask);
			threads[i*sizeofservers+j].SetTask(lTask);
		}

	}

	gettimeofday(&mid, NULL);
	for (int i = 0; i < (argc-2)*sizeofservers; i++) {
		threads[i].Start();
	}
	for (int i = 0; i < (argc-2)*sizeofservers; i++) {
		threads[i].ThreadJoin();
	}
	delete [] threads;
	gettimeofday(&end, NULL);
	printf("time consumed since threads creation till threads finish: %ld us\n",
	       end.tv_usec - start.tv_usec + (end.tv_sec -
					      start.tv_sec) * 1000000);
	printf("time consumed since threads send till threads finish: %ld us\n",
	       end.tv_usec - mid.tv_usec + (end.tv_sec -
					      mid.tv_sec) * 1000000);
	
	if(gInput != NULL)
	{
		delete []gInput; // which was allocated in ReadFileToSingleBuffer
		gInput = NULL;
	}
	return 0;
}
