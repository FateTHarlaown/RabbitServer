#include "EventLoop.h"
#include <boost/bind.hpp>
#include <sys/timerfd.h>
#include "Acceptor.h"
#include <iostream>

using namespace Rabbit::net;
using namespace std;

void timercall()
{
	cout << "I'm timer!!yaho!!!!" << endl;
}

int main()
{
	EventLoop loop;
	Acceptor ac(&loop, "INADDR_ANY", 65432);
	ac.registAcceptor();
	loop.addTimerRunEvery(1, boost::bind(&timercall));
	loop.loop();
	return 0;
}


