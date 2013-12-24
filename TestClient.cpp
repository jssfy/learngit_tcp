#include "ThreadManager.h"
#include <stdio.h>
using namespace KIDD;

int main()
{
	WorkerThread *lThread =
	    new WorkerThread(new
			     UDPClientTask("localhost", "filesize_req",
					   "hello world!"));
	lThread->Start();
	lThread->ThreadJoin();

	return 0;
}
