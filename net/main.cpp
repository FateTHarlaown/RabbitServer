#include <boost/bind.hpp>
#include <iostream>
#include "TcpServer.h"
#include "EventLoop.h"
#include "NetAddr.h"

using namespace Rabbit::net;
using namespace std;

void onConnection(const ConnectionPtr & conn)
{
	cout << "New connection come! yaho!" << endl;
}

void onMessage(const ConnectionPtr & conn, const char * data, const ssize_t len)
{
	cout << "received " << len << "Bytes" << endl;
	for(int i = 0; i < len; i++)
	{
		cout << data[i];
	}
}

int main()
{
	EventLoop loop;
	NetAddr listenAddr("0.0.0.0", 54321);
	
	TcpServer myServer(&loop, listenAddr, "yoooo~!");
	myServer.setConnectionCallback(onConnection);
	myServer.setMessageCallback(onMessage);
	myServer.start();
	loop.loop();
	return 0;
}


