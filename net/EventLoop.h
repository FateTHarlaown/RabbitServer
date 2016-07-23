#ifndef RABBIT_NET_EVENT_LOOP_H
#define RABBIT_NET_EVENT_LOOP_H

#include <boost/scoped_ptr.hpp>
#include "../base/CallBacks.h"
#include <vector>
#include <boost/noncopyable.hpp>
#include "Poller.h"
#include "TimerQueue.h"
#include "Timestamp.h"

namespace Rabbit{
namespace net{
class Channel;
class EventLoop:boost::noncopyable
{
public:
	typedef std::vector<Channel*> ChannelList;
	EventLoop();
	void upDateChannel(Channel * channel);
	void removeChannel(Channel * channel);
	//start eventLoop
	void loop();
	void stop();
	//add a timer in a certain time
	void addTimerRunAt(const Timestamp & time, const TimerCallBack & func);
	//add a timer, run after delay seconds
	void addTimerRunAfter(double delay, const TimerCallBack & func);
	//add a timer, run evevry interval seconds
	void addTimerRunEvery(double interval, const TimerCallBack & func);

private:
	ChannelList activeChannels;
	bool quit_;
	boost::scoped_ptr<Poller> poller_;
	boost::scoped_ptr<TimerQueue> timerQueue_;
};

}
}

#endif
