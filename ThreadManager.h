#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <pthread.h>
#include <string>
using namespace std;

namespace KIDD {

#define _DEBUG

#ifdef _DEBUG
#define P(x) printf x
#else
#define P(x) 
#endif

#define DEFAULT_PORT 15800
	
	class WorkerTask {
public:
		WorkerTask():mStop(false) {
		};
		virtual void Run();
		void SetStop() {
			mStop = true;
		};
		bool TestStop() {
			return mStop;
		}
		virtual ~ WorkerTask() {
		}
private:
		bool mStop;
	};

	class WorkerThread {
public:
		static void *ThreadFunction(void *);
		void Run();
		bool Init();
		virtual void Init2() {
		};
		void Start();
		void Destroy();
		void WaitForever();
		void CancelTask();
		void SetTask(WorkerTask* ipTask);
		WorkerThread(WorkerTask * ipTask=NULL);
		~WorkerThread();
		bool mSignaled;
		void ThreadJoin();
private:
		pthread_mutex_t mMutex;
		pthread_cond_t mEvent;
		pthread_t mThreadID;
		WorkerTask *mpTask;
	};

	class ThreadManager {
public:
		void CheckIn();
		void CheckOut();
	};

	class UDPServerTask:public WorkerTask {
public:
		virtual void Run();
	};

	class UDPClientTask:public WorkerTask {
public:
		UDPClientTask(const char *iServerName, const char *iMesgType,
			      const char *iMesg)
		:mServerName(iServerName), mMesgType(iMesgType), mMesg(iMesg) {
		};
		//UDPClientTasK(const char * iServerName, const char * iMesgType, const char * iMesg){};
		virtual void Run();
private:
		string mServerName;
		string mMesgType;
		string mMesg;
	};
	class TCPClientTask:public WorkerTask {
public:
		TCPClientTask(const char *iServerName);
		TCPClientTask(const char *iServerName, short mPort);
	~TCPClientTask();
		void Run();
		virtual void Init2();
private:
		 string mServerName;
		struct hostent *he;
		short mPort;
		int mSockfd;
		char *mInput;
	};
	class TCPClientThreadPoolTask:public WorkerTask {
public:
		TCPClientThreadPoolTask() {
		} void Run();
private:
		char *mServerName;

};
	char *PopServerQ();
	void PushServerQ(char *ipServer);
	void InitGlobals();

	// maintaining a Q of connected sockets
	int PopConnectionQ();
	void PushConnectionQ(int iSockfd);
	int ConnectToServer(char*ipServer, short iPort);

	class TCPServerTask:public WorkerTask{
public:
	void Run();
	TCPServerTask();
	TCPServerTask(short iPort);
private:
	short mPort; 
//	string mServerName;
	};

}
#endif
