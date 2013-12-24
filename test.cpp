#include "ThreadManager.h"
#include <stdio.h>
using namespace KIDD;
#include <iostream>
#include <string>
using namespace std;

pthread_cond_t gEvent;
pthread_mutex_t gMutex;

void *TF(void *ipThread)
{
	//WorkerThread* lpThread = reinterpret_cast<WorkerThread*>(ipThread);
	//lpThread->Run();

	//sleep(2);
	pthread_mutex_lock(&gMutex);
	cout << "to wait" << endl;
	pthread_cond_wait(&gEvent, &gMutex);
	pthread_mutex_unlock(&gMutex);
	cout << "thread function\n";
}

void *TF2(void *ipThread)
{
	//WorkerThread* lpThread = reinterpret_cast<WorkerThread*>(ipThread);
	//lpThread->Run();
	pthread_mutex_lock(&gMutex);
	pthread_cond_signal(&gEvent);
	cout << "signaled" << endl;
	pthread_mutex_unlock(&gMutex);
	cout << "thread function 2\n";
}

int main()
{
	//  pthread_mutex_init(&gMutex, NULL);
	//  pthread_cond_init(&gEvent, NULL);

	printf("test:\n");
	WorkerThread *lThread = new WorkerThread(new WorkerTask());
	//  sleep(4);
	lThread->Start();
	//  sleep(1);

	/*          pthread_t id, id2;
	   int hr;
	   if(hr = pthread_create(&id, NULL, TF, NULL)){}
	   //if(hr = pthread_create(&id2, NULL, TF2, NULL)){}
	   //sleep(1);
	   pthread_mutex_lock(&gMutex);
	   pthread_cond_signal(&gEvent);
	   cout << "signaled" << endl;
	   pthread_mutex_unlock(&gMutex);
	   pthread_join(id, NULL);
	   //       pthread_join(id2, NULL);
	   //if(hr = pthread_create(&id, NULL, TF, NULL)){}
	 */
	string lInput;
	char lStart;
	do {
		cout << "<S>top: ";
		getline(std::cin, lInput);
		if (lInput.size() == 0)
			continue;
		lStart = lInput.at(0);
		if (lStart == 'S')
			break;
	}
	while (true);

	int i = 9;

	lThread->CancelTask();

	return 0;
}
