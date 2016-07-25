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

//register it's channel to EventLoop, TimerQueue start to work
void TimerQueue::TimerQueueStart()
{
	struct itimerspec how;
	bzero(&how, sizeof how);
	how.it_value.tv_nsec = NANOITER;
	how.it_interval.tv_nsec = NANOITER; 
	timerChannel_.update();
	timerfd_settime(timerFD_, 0, &how, NULL);
}

//copy expired timers to a vector, and erase them
void TimerQueue::getExpiredTimers()
{
	expiredTimers_.clear();
	if(!timers_.empty())
	{
		Timestamp now = Timestamp::now();
		TimerList::iterator it = timers_.lower_bound(now);
		assert(it == timers_.end() || now < it->first || now == it->first);
		//copy the expired timers to a vector, and delete it's pointer from map
		copy(timers_.begin(), it, back_inserter(expiredTimers_));
		timers_.erase(timers_.begin(), it);
	}
}

void TimerQueue::resetTimers()
{
	std::vector<Entry>::iterator it;
	for(it = expiredTimers_.begin(); it != expiredTimers_.end(); it++)
	{
		it->second->run();
		//if this timers will repeat, reset it and add it to timerQueue
		if(it->second->isRepeated())
		{
			it->second->reset();
			addTimer(it->second);
		}
		else//free the expired timers
		{
			delete it->second;
		}
	}
}

//this function need to be binded to the channel as it's read callback function
void TimerQueue::handleRead()
{
	getExpiredTimers();
	resetTimers();
}

//add a new timer
bool TimerQueue::addTimer(Timer * timer)
{
	Timestamp expirationTime = timer->getExpiration();
	if(expirationTime < Timestamp::now())
		return false;
	timers_.insert(TimerList::value_type(expirationTime, timer));
	return true;
}


//add a timer in IO thread
void TimerQueue::addTimerInLoop(Timer * timer)
{
	loop_->RunInLoop(boost::bind(&TimerQueue::addTimer, this, timer));
}

TimerQueue::~TimerQueue()
{
	TimerList::iterator it = timers_.begin();
	TimerList::iterator end = timers_.end();
	//delete all the timers in heap
	for( ; it != end; it++)
	{
		delete it->second;
	}
}
