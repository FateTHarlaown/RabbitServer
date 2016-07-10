#include <sys/epoll.h>
#include "Channel.h"
#include "EventLoop.h"
using namespace Rabbit;
using namespace Rabbit::net;

static const int kNew = 0;
static const int kAdded = 1;
const int Channel::NoEvent = 0;
const int Channel::ReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::WriteEvent = EPOLLOUT;

Channel::Channel(EventLoop * loop, int fd):loop_(loop), fd_(fd), index_(kNew), events_(0), revents_(0)
{
}

void Channel::update()
{
	loop_->upDateChannel(this);
}

void Channel::handleEvent()
{
	if(revents_ & EPOLLERR)
		errorCallback();
	if(revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
		readCallback();
	if(revents_ & EPOLLOUT)
	  writeCallback();
}

