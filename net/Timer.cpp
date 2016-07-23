#include "Timer.h"
#include <stdio.h>

using namespace Rabbit;
using namespace Rabbit::net;

Timer::Timer(TimerCallBack callBack, Timestamp expiration, bool repeat, int interval) : callBack_(callBack), expiration_(expiration), repeat_(repeat), interval_(interval)
{

}

void Timer::reset()
{
	if(repeat_)
	{
		expiration_ = Timestamp(Timestamp::nowAfter(interval_));
	} 
	else
	{
		printf("this timer can not restart, ignore it\n");
	}
}

void Timer::run()
{
	callBack_();
}

bool Timer::isRepeated()
{
	return repeat_;
}

Timestamp Timer::getExpiration() 
{
	return expiration_;
}

