#include "ThreadManager.h"
#include <stdio.h>
using namespace KIDD;

int main()
{
	WorkerThread *lThread = new WorkerThread(new UDPServerTask());
	lThread->Start();
	lThread->ThreadJoin();
	//new UDPServerTask();
	//new WorkerTask();

	return 0;
}
