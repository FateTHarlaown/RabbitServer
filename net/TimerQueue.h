#ifndef RABBIT_NET_TIMERQUEUE
#define RABBIT_NET_TIMERQUEUE

#include "../base/Timestamp.h"
#include <map>
#include <vector>
#include "Channel.h"

namespace Rabbit{
namespace net{

using namespace Rabbit::base;
class Timer;
class EventLoop;
class Channel;
class TimerQueue
{
public:
	typedef std::multimap<Timestamp, Timer *> TimerList;
	typedef boost::function<void ()> TimerCallBack;
	TimerQueue(EventLoop * loop);
	//in this destructor, the channel shoud be remove and Timers were delete
	~TimerQueue();
	//add a timer to TimerQueue
	bool addTimer(Timestamp time, TimerCallBack & cb, int interval);
private:
	void handleRead();
	void resetTimers(std::vector<Timer *> expiredTimers);
	TimerList timers_;
	int timerFD_;
	EventLoop * loop_;
	Channel timerChannel_;
	std::vector<Timer *> expiredTimers;		
};
}
}

#endif
