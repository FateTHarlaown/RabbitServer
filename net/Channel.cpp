#include "Channel.h"
#include <sys/epoll.h>

using namespace Rabbit;
using namespace Rabbit::net;

const int Channel::NoEvent = 0;
const int Channel::ReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::WriteEvent = EPOLLOUT;

Channel::Channel(EventLoop * loop, int fd):loop_(loop), fd_(fd), index_(-1), events_(0), revents_(0)
{
	
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

