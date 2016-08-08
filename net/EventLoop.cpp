#include "EventLoop.h"
#include <stdio.h>
#include <boost/bind.hpp>
#include <string>
#include "Poller.h"
#include <assert.h>
#include <sys/eventfd.h>
#include "Channel.h"
#include "Timer.h"
#include "../base/Gettid.h"

using namespace Rabbit;
using namespace Rabbit::net;
using namespace Rabbit::base;

__thread EventLoop * t_loopInThisThread = NULL;

EventLoop::EventLoop():wakeup_fd_(eventfd(0, 0)), 
						threadId_(gettid()),
						quit_(false), 
						callingPendingFunctors_(false),
						poller_(new Poller(this)), 
						timerQueue_(new TimerQueue(this)),
						wakeupChannel_(new Channel(this, wakeup_fd_))
{
	if(t_loopInThisThread != NULL)//one thread can only has one EventLoop
	{
		printf("there is another EventLoop");
		throw std::string("has another loop");
	}
	t_loopInThisThread = this;
	timerQueue_->TimerQueueStart();	
	wakeupChannel_->setReadCallBack(boost::bind(&EventLoop::handleRead, this));
	wakeupChannel_->enableReading();
	wakeupChannel_->update();
}

EventLoop::~EventLoop()
{
	wakeupChannel_->remove();
	close(wakeup_fd_);
	t_loopInThisThread = NULL;
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
		doPendingFunctors();
	}
}

void EventLoop::stop()
{
	quit_ = true;
	if(!isInLoopThread())
		wakeup();
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
	timerQueue_->addTimerInLoop(new Timer(func, time, false, 0));
}

//add a timer, run after delay seconds
void EventLoop::addTimerRunAfter(double delay, const TimerCallBack & func)
{
	timerQueue_->addTimerInLoop(new Timer(func, Timestamp::nowAfter(delay), false, 0));
}

//add a timer, run evevry interval seconds
void EventLoop::addTimerRunEvery(double interval, const TimerCallBack & func)
{
	timerQueue_->addTimerInLoop(new Timer(func, Timestamp::nowAfter(interval), true, interval));
}

//is this thread has this EventLoop
bool EventLoop::isInLoopThread()
{
	return threadId_ == gettid();
}

//this function can make the callback function run in EventLoop Thread
void EventLoop::RunInLoop(const Functors & func)
{
	if(isInLoopThread())
		func();
	else
		QueueInLoop(func);
}

//insert a callback function to pendingFunctions
void EventLoop::QueueInLoop(const Functors & func)
{
	{
		MutexLockGuard lock(mutex_);
		pendingFunctors_.push_back(func);
	}
	if(!isInLoopThread() || callingPendingFunctors_)
		wakeup();		
}

//send something to wakeupfd_ to wake EventLoop Thread
void EventLoop::wakeup()
{
	uint64_t num = 1;
	int ret = write(wakeup_fd_, &num, sizeof num);
	if(ret != sizeof num)
	{
		perror("send to wake up io thread failed");
	}
}

//the callback function for wakeupChannel_
void EventLoop::handleRead()
{
	uint64_t num;
	int ret = read(wakeup_fd_, &num, sizeof num);
	if(ret != sizeof num)
	{
		perror("read to wake up io thread failed");
	}
	printf("be wakeup\n");
}

void EventLoop::doPendingFunctors()
{
	std::vector<Functors> toRun;
	callingPendingFunctors_ = true;
	{
		MutexLockGuard lock(mutex_);
		toRun.swap(pendingFunctors_);
		pendingFunctors_.clear();
	}

	for(size_t i = 0; i < toRun.size(); i++)
	{
		toRun[i]();
	}
	callingPendingFunctors_ = false;
}

void EventLoop::assertInLoopThread()
{
	assert(isInLoopThread());
}
