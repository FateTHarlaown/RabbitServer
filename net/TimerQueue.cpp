#include "TimerQueue.h"
#include "EventLoop.h"
#include <sys/timerfd.h>
#include <iterator>
#include <assert.h>
#include "Timer.h"
#include <boost/bind.hpp>

using namespace Rabbit;
using namespace Rabbit::net;

TimerQueue::TimerQueue(EventLoop * loop): timerFD_(timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC)), loop_(loop), timerChannel_(loop, timerFD_)
{
	timerChannel_.setReadCallBack(boost::bind(&TimerQueue::handleRead, this));
	timerChannel_.enableReading();
}

void TimerQueue::TimerQueueStart()
{
	struct itimerspec how;
	bzero(&how, sizeof how);
	how.it_value.tv_nsec = NANOITER;
	how.it_interval.tv_nsec = NANOITER; 
	timerChannel_.update();
	timerfd_settime(timerFD_, 0, &how, NULL);
}

void TimerQueue::getExpiredTimers()
{
	expiredTimers_.clear();
	if(!timers_.empty())
	{
		Timestamp now = Timestamp::now();
		TimerList::iterator it = timers_.lower_bound(now);
		assert(it == timers_.end() || now <= it->first);
		copy(timers_.begin(), it, back_inserter(expiredTimers_));
	}
}

void TimerQueue::resetTimers()
{
	std::vector<Entry>::iterator it;
	for(it = expiredTimers_.begin(); it != expiredTimers_.end(); it++)
	{
		it->second->run();
		if(it->second->isRepeated())
			it->second->reset();
	}
}


