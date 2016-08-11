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

string message = "haaaahahahahahahahahahahahaahhahahahahahahahahahahahahahahahahhahahahahaahhahahahahahahahhahahahahahahahahhahahahahahhahaahahahhahahahahahahhahahahahahahahahahahahahhahahahahahhaahahaaahhahahaahahhahahahahahahaahhahahahahahahhahahahahahhahahahahahahahshshshshshshshshhshshshshshshjksdfjhklsghjkserhguilwerhjgiojewoakjwekbjnjfkhejkdvhgjifehrgjkflhdfskgjkldfsjgko;rslhgijlewrnlkgkldfshgkjsdfgklhagilkrejflkawehjgjkadfklghrikeahjfgjkdfhk;larhjgnl;fjkioerwhjfkjaewhgkjlaehgkjraelfkl;wehj;lkghk;asdjfkdthgfk;aeshgfkjdashgjkawdljfkl;hweA;GHNFKDSHFGNIKWAFHKLADWSFKLEH;Efhsdlkjfkldajfkldhjsaglkjmroieakjfnkladsjtglkarsjglkasjgklaj;d;lfojlfkad;jgklcjklrehjfglknraejkgvnakjfghikdfjsgjjjjjjjjjjjjjjjjjjjjjlssssssssssssssssssssssssgjjjjjjjjjjjjjjjjjja;;;;;;;;;;gjjjjjjjjjjjjjjjjjjjaa23333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333\nendl!\n";

string message2 = "555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555\nend!\n";

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
	conn->send(message);
	conn->send(message2);
	conn->closeConnection();
}


int main()
{
	EventLoop loop;
	NetAddr listenAddr("0.0.0.0", 54321);
	
	TcpServer myServer(&loop, listenAddr, "yoooo~!");
	myServer.setConnectionCallback(onConnection);
	myServer.setMessageCallback(onMessage);
	myServer.setThreadNum(5);
	myServer.start();
	loop.loop();
	return 0;
}
//test buffer
/*
int main()
{
	Buffer buffer_;
	buffer_.append("123456789",9);
	cout << buffer_.retrieveAllAsString().c_str() << endl;
}
*/
