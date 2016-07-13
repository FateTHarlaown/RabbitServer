#include "EventLoop.h"
#include <sys/timerfd.h>
#include "Acceptor.h"
#include <iostream>

using namespace Rabbit::net;
using namespace std;

int main()
{
	EventLoop loop;
	Acceptor ac(&loop, "INADDR_ANY", 65432);
	ac.registAcceptor();
	loop.loop();
	return 0;
}


