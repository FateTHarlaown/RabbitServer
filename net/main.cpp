#include "EventLoop.h"
#include "EventLoopThread.h"
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
	sleep(4);
	while(g_loop == NULL);
	g_loop->addTimerRunEvery(2, boost::bind(&timerCall));
	printf("I add a cb!\n");
	return NULL;
}


int main()
{
	pthread_t id;
	pthread_create(&id, NULL, timerThread, NULL);
	EventLoopThread EVLT("zhang");
	g_loop = EVLT.startLoop(); 
	while(1);
	return 0;
}


