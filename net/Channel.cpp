#include <sys/epoll.h>
#include <stdio.h>
#include "Channel.h"
#include "EventLoop.h"
using namespace Rabbit;
using namespace Rabbit::net;

static const int kNew = 0;
static const int kAdded = 1;
const int Channel::NoEvent = 0;
const int Channel::ReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::WriteEvent = EPOLLOUT;

Channel::Channel(EventLoop * loop, int fd):loop_(loop), fd_(fd), index_(kNew), events_(0), revents_(0), tied_(false), writing_(false)
{
}

void Channel::update()
{
	loop_->upDateChannel(this);
}

void Channel::handleEvent()
{
	if(tied_ == true)
	{

		boost::shared_ptr<void> guard;
		guard = tie_.lock();
		if(guard)
		{
			handleEventWithGuard();
		}
	}
	else
	{
		handleEventWithGuard();
	}
}
void Channel::handleEventWithGuard()
{
		if(revents_ & EPOLLERR)
		{
			printf("error happened in %ld\n", gettid());
			if(errorCallback)
				errorCallback();
		}
		if(revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP | EPOLLHUP))
		{
			if(revents_ & EPOLLHUP)
			{
				printf("EPOLLHUP now will close Connection in %ld\n", gettid());
				if(hupCallback)
					hupCallback();
			}
			else
			{
				if(readCallback)
					readCallback();
			}
		}
		if(revents_ & EPOLLOUT)
		{
			printf("now to write!");
			if(writeCallback)
				writeCallback();
		}
}

void Channel::remove()
{
	disableAll();
	loop_->removeChannel(this);
}
