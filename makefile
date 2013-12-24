DEBUG = -g
test: test.o ThreadManager.o
	g++ -o test test.o ThreadManager.o -pthread
test.o: test.cpp ThreadManager.h
	g++ $(DEBUG) -c test.cpp 
ThreadManager.o: ThreadManager.cpp ThreadManager.h
	g++ $(DEBUG) -c ThreadManager.cpp 
UDPServerTask.o: UDPServerTask.cpp ThreadManager.h
TCPServerTask.o: TCPServerTask.cpp ThreadManager.h
	g++ $(DEBUG) -c TCPServerTask.cpp -pthread
UDPClientTask.o: UDPClientTask.cpp ThreadManager.h
	g++ $(DEBUG) -c UDPClientTask.cpp
TestServer.o: TestServer.cpp ThreadManager.h share.h
	g++ $(DEBUG) -c TestServer.cpp
TestServer: TestServer.o ThreadManager.o UDPServerTask.o
	g++ -o TestServer TestServer.o ThreadManager.o UDPServerTask.o -pthread
TestClient.o: TestClient.cpp ThreadManager.h share.h
	g++ $(DEBUG) -c TestClient.cpp
TestClient: TestClient.o ThreadManager.o UDPClientTask.o
	g++ -o TestClient TestClient.o ThreadManager.o UDPClientTask.o -pthread
tcpclient: TCPClient.cpp
	g++ -o tcpclient TCPClient.cpp
tcpserver: TCPServer.cpp tools.o
	g++ -o tcpserver TCPServer.cpp tools.o
TCPServer: TCPServer.cpp tools.o TCPServerTask.o
	g++ $(DEBUG) -o TCPServer TCPServer.cpp tools.o ThreadManager.o TCPServerTask.o -pthread
TCPClientTask.o: TCPClientTask.cpp ThreadManager.h 
	g++ $(DEBUG) -c TCPClientTask.cpp 
TCPClientThreadPoolTask.o: TCPClientThreadPoolTask.cpp ThreadManager.h tools.o
	g++ $(DEBUG) -c TCPClientThreadPoolTask.cpp tools.o
testtcpclient.o: testtcpclient.cpp ThreadManager.h
	g++ $(DEBUG) -c testtcpclient.cpp
testtcpclientthreadpool.o: testtcpclientthreadpool.cpp ThreadManager.h
	g++ $(DEBUG) -c testtcpclientthreadpool.cpp
testtcpclient: testtcpclient.o ThreadManager.o TCPClientTask.o tools.o
	g++ -o testtcpclient testtcpclient.o ThreadManager.o TCPClientTask.o -pthread tools.o
testtcpclientthreadpool: testtcpclientthreadpool.o ThreadManager.o TCPClientThreadPoolTask.o
	g++ -o testtcpclientthreadpool testtcpclientthreadpool.o ThreadManager.o TCPClientThreadPoolTask.o -pthread tools.o
tcpclientsinglethread: tcpclientsinglethread.cpp tools.o ThreadManager.h
	g++  $(DEBUG) -o tcpclientsinglethread tcpclientsinglethread.cpp tools.o
clean:
	rm -f *.o test TestServer TestClient
main: tools.o main.cpp
	g++ -o main  -g main.cpp tools.o
tools.o: tools.cpp
	g++ -o tools.o -g -c tools.cpp
