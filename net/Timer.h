#ifndef RABBIT_NET_TIMER
#define RABBIT_NET_TIMER

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include "Timestamp.h"

namespace Rabbit{
namespace net{
class Timer:boost::noncopyable
{
public:
	typedef boost::function<void ()> TimerCallBack;
	Timer(TimerCallBack callBack, Timestamp expiration, bool repeat, int interval);
	//call the callBack function when the timer expired
	void run();
	bool isRepeated();
	void reset();
	Timestamp getExpiration();

private:
	TimerCallBack callBack_;
	Timestamp expiration_;
	const bool repeat_;
	const int interval_;
};

}
}

#endif
