#ifndef RABBIT_NET_EVENT_LOOP_H
#define RABBIT_NET_EVENT_LOOP_H

#include <boost/atomic.hpp>
#include <boost/scoped_ptr.hpp>
#include "../base/CallBacks.h"
#include <vector>
#include <boost/noncopyable.hpp>
#include "Poller.h"
#include "TimerQueue.h"
#include "Timestamp.h"
#include "../base/Mutex.h"

namespace Rabbit{
namespace net{
class Channel;
class EventLoop:boost::noncopyable
{
public:
	typedef std::vector<Channel*> ChannelList;
	EventLoop();
	~EventLoop();
	void upDateChannel(Channel * channel);
	void removeChannel(Channel * channel);
	//start eventLoop
	void loop();
	void stop();
	//is this thread has this EventLoop
	bool isInLoopThread();
	//for debug
	void assertInLoopThread();
	//add a timer in a certain time
	void addTimerRunAt(const Timestamp & time, const TimerCallBack & func);
	//add a timer, run after delay seconds
	void addTimerRunAfter(double delay, const TimerCallBack & func);
	//add a timer, run evevry interval seconds
	void addTimerRunEvery(double interval, const TimerCallBack & func);
	//this function can make the callback function run in EventLoop Thread
	void RunInLoop(const Functors & func);

private:
	int wakeup_fd_;
	pid_t threadId_;	
	ChannelList activeChannels;
	boost::atomic<bool> quit_;
	boost::atomic<bool> callingPendingFunctors_;
	boost::scoped_ptr<Poller> poller_;
	boost::scoped_ptr<TimerQueue> timerQueue_;
	boost::scoped_ptr<Channel> wakeupChannel_;
	Rabbit::base::MutexLock mutex_;
	std::vector<Functors> pendingFunctors_;
private:
	//the callback function for wakeupChannel_
	void handleRead();
	void doPendingFunctors();
	//insert a callback function to pendingFunctions
	void QueueInLoop(const Functors & func);
	//send something to wakeupfd_ to wake EventLoop Thread
	void wakeup();

};

}
}

#endif
