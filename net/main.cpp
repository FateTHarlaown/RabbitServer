#include <boost/bind.hpp>
#include <iostream>
#include "TcpServer.h"
#include "TcpConnection.h"
#include "EventLoop.h"
#include "NetAddr.h"
#include "Buffer.h"
#include "Timestamp.h"

using namespace Rabbit::net;
using namespace std;

void onConnection(const ConnectionPtr & conn)
{
	cout << "New connection come! yaho!" << endl;
}

void onMessage(const ConnectionPtr & conn, Buffer * buffer, Timestamp receiveTime)
{
	cout << "Get " << buffer->readbleBytes() << " Bytes" << endl;	
	cout << "Connection name " << conn->name() << endl;
	cout << "Time :" << receiveTime.toString() << endl;
	cout << "That content: " << buffer->retrieveAllAsString() << endl;
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


