#include "ThreadManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <queue>
#include <sys/time.h>

using namespace std;
extern pthread_cond_t gEvent;
extern pthread_mutex_t gMutex;

namespace KIDD {

	void WorkerTask::Run() {
		while (!TestStop()) {
			printf("WorkerTask::Run\n");
			sleep(10);
		} printf("out of WorkerTask::Run\n");
	}
WorkerThread::WorkerThread(WorkerTask * ipTask):mSignaled(false) {
		mpTask = ipTask;
		//Init();
		//      gSignaled = true;
	}

	void WorkerThread::CancelTask() {
		if (mpTask) {
			mpTask->SetStop();
			cout << "set stop" << endl;
		} else
			cout << "??\n";
		pthread_join(mThreadID, NULL);
	}
	void WorkerThread::SetTask(WorkerTask* ipTask)
	{
		if(mpTask != NULL)
		{
			delete mpTask;
		}
		mpTask = ipTask;
	}
	bool WorkerThread::Init() {
		int hr = 0;
		hr = pthread_mutex_init(&mMutex, NULL);
		if (hr) {
			printf("mutex init failure\n");
			return false;
		}
		pthread_cond_init(&mEvent, NULL);
		if (hr) {
			printf("cond init failure\n");
			return false;
		}

		if (hr = pthread_create(&mThreadID, NULL, ThreadFunction, this)) {
			printf("failed in creating a worker thread\n");
			return false;
		}

		return true;
	}
	void WorkerThread::WaitForever() {
		pthread_mutex_lock(&mMutex);
		//sleep(1);
		//cout << "start waiting forever" << pthread_self() << endl ;
		if (!mSignaled)	// if not using mSignaled, it seems the below function will block if pthread_cond_signal is triggered before this thread comes here
			pthread_cond_wait(&mEvent, &mMutex);
		//cout << "finished waiting forever in thread: " << pthread_self() << endl;
		pthread_mutex_unlock(&mMutex);

	}
	void *WorkerThread::ThreadFunction(void *ipThread) {
		WorkerThread *lpThread =
		    reinterpret_cast < WorkerThread * >(ipThread);
		lpThread->Run();
	}
	void WorkerThread::Run() {
		WaitForever();
		mpTask->Run();
	}
	void WorkerThread::Start() {
		Init();
		pthread_mutex_lock(&mMutex);
		pthread_cond_signal(&mEvent);
		mSignaled = true;
		pthread_mutex_unlock(&mMutex);
	}
	WorkerThread::~WorkerThread() {
		Destroy();
	}
	void WorkerThread::Destroy() {
		::pthread_cond_destroy(&mEvent);
		::pthread_mutex_destroy(&mMutex);
		if (!mpTask) {
			delete mpTask;
			mpTask = NULL;
			cout << "mpTask now NULL \n";
		}
	}
	void WorkerThread::ThreadJoin() {
		if (mThreadID)
			pthread_join(mThreadID, NULL);
	}

	typedef queue < char *>ServerAddrQ;
	ServerAddrQ gServerQ;
	typedef queue <short>ConnectionQ;
	ConnectionQ gConnectionQ;
	// below mutex must be initialized before calling the following 2 functions
	pthread_mutex_t gServerQMutex;
	char *PopServerQ() {
		pthread_mutex_lock(&gServerQMutex);
		if (gServerQ.empty()) {
			pthread_mutex_unlock(&gServerQMutex);
			return NULL;
		}
		char *lpServer = gServerQ.front();
		gServerQ.pop();
		pthread_mutex_unlock(&gServerQMutex);
		return lpServer;
	}
	void PushServerQ(char *ipServer) {
		pthread_mutex_lock(&gServerQMutex);
		gServerQ.push(ipServer);
		pthread_mutex_unlock(&gServerQMutex);
	}
	void InitGlobals() {

		int hr = pthread_mutex_init(&gServerQMutex, NULL);
		if (hr) {
			perror("gServerQMutex");
		}
	}
	void DestroyGlobals() {
		::pthread_mutex_destroy(&gServerQMutex);
	}

	// maintaining a Q of connected sockets
	int PopConnectionQ()
	{
		pthread_mutex_lock(&gServerQMutex);
		if(gConnectionQ.empty())
		{
			pthread_mutex_unlock(&gServerQMutex);
			return -1;
		}
		short lSockfd = gConnectionQ.front();
		gConnectionQ.pop();
		pthread_mutex_unlock(&gServerQMutex);
		return lSockfd;
	}
	void PushConnectionQ(int iSockfd)
	{
		pthread_mutex_lock(&gServerQMutex);
		gConnectionQ.push(iSockfd);
		pthread_mutex_unlock(&gServerQMutex);
	}
}
