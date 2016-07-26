#ifndef RABBIT_NET_TIMERQUEUE
#define RABBIT_NET_TIMERQUEUE

#include "Timestamp.h"
#include <map>
#include <vector>
#include <boost/noncopyable.hpp>
#include "Channel.h"

namespace Rabbit{
namespace net{

class Timer;
class EventLoop;
class Channel;
class TimerQueue:boost::noncopyable
{
public:
	typedef std::pair<Timestamp, Timer *> Entry;
	typedef std::multimap<Timestamp, Timer *> TimerList;
	typedef boost::function<void ()> TimerCallBack;
	TimerQueue(EventLoop * loop);
	//in this destructor, the channel shoud be remove and exists Timers were delete
	~TimerQueue();
	//up date the timerChannel_ into the event loop
	void TimerQueueStart();
	//add a timer to TimerQueue
	bool addTimer(Timer * timer);
	//add a timer in IO thread
	void addTimerInLoop(Timer * timer);
private:
	//bind to the timerChannel_ as readCallBack function
	void handleRead();
	//get expired timers, saved it into expiredTimers	
	void getExpiredTimers();
	//reset the expired timers, if it not repeat, delete it
	void resetTimers();
	TimerList timers_;
	int timerFD_;
	EventLoop * loop_;
	Channel timerChannel_;
	std::vector<Entry> expiredTimers_;		
	static const int NANOITER = 100000000;
};
}
}

#endif
