#include "EventLoop.h"
#include <stdio.h>
#include <boost/bind.hpp>
#include <sys/timerfd.h>
#include <iostream>
#include <pthread.h>

using namespace Rabbit::net;
using namespace std;

EventLoop * g_loop = NULL;
void timerCall()
{
	printf("yaho!!timer!!\n");
}
void * timerThread(void * arg)
{
	pthread_detach(pthread_self());
	sleep(1);
	g_loop->addTimerRunEvery(2, boost::bind(&timerCall));
	printf("I add a cb!\n");
	return NULL;
}


int main()
{
	EventLoop loop;
	g_loop = &loop;
	pthread_t t;
	pthread_create(&t, NULL, timerThread, NULL);
	loop.loop();
	return 0;
}


