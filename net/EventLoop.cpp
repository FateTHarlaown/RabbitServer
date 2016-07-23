#include "EventLoop.h"
#include "Poller.h"
#include <assert.h>
#include "Channel.h"
#include "Timer.h"

using namespace Rabbit;
using namespace Rabbit::net;

EventLoop::EventLoop():quit_(false), poller_(new Poller(this)), timerQueue_(new TimerQueue(this))
{
	timerQueue_->TimerQueueStart();	
}

void EventLoop::loop()
{
	assert(quit_ == false);
	while(quit_ != true)
	{
		activeChannels.clear();
		poller_->Poll(-1, &activeChannels);
		std::vector<Channel*>::iterator it;
		for(it = activeChannels.begin(); it != activeChannels.end(); it++)
		{
			(*it)->handleEvent();
		}
	}
}

void EventLoop::stop()
{
	quit_ = true;
}

//update or add a channel
void EventLoop::upDateChannel(Channel * channel)
{
	assert(channel != NULL);
	poller_->upDateChannel(channel);
}

//remove the channel
void EventLoop::removeChannel(Channel * channel)
{
	poller_->removeChannel(channel);
}

//add a timer in a certain time
void EventLoop::addTimerRunAt(const Timestamp & time, const TimerCallBack & func)
{
	timerQueue_->addTimer(new Timer(func, time, false, 0));
}

//add a timer, run after delay seconds
void EventLoop::addTimerRunAfter(double delay, const TimerCallBack & func)
{
	timerQueue_->addTimer(new Timer(func, Timestamp::nowAfter(delay), false, 0));
}

//add a timer, run evevry interval seconds
void EventLoop::addTimerRunEvery(double interval, const TimerCallBack & func)
{
	timerQueue_->addTimer(new Timer(func, Timestamp::nowAfter(interval), true, interval));
}
