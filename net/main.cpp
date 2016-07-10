#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"
#include <sys/timerfd.h>
#include <boost/bind.hpp>
#include <iostream>

using namespace Rabbit::net;
using namespace std;

EventLoop * gloop = NULL;

void timeout()
{
	cout << "TIME OUT !" << endl;
	gloop->stop();
}

int main()
{
	EventLoop loop;
	gloop = &loop;
	int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	Channel channel(&loop, timerfd);
	channel.setReadCallBack(timeout);
	channel.enableReading();
	channel.update();
	struct itimerspec howlong;
	bzero(&howlong, sizeof(howlong));
	howlong.it_value.tv_sec = 5;
	timerfd_settime(timerfd, 0, &howlong, NULL);

	loop.loop();
	close(timerfd);

	return 0;
}


