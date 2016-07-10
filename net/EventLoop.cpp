#include "EventLoop.h"
#include "Poller.h"
#include <assert.h>
#include "Channel.h"

using namespace Rabbit;
using namespace Rabbit::net;

EventLoop::EventLoop():quit_(false), poller_(new Poller(this))
{
		
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
	}
}

void EventLoop::stop()
{
	quit_ = true;
}

void EventLoop::upDateChannel(Channel * channel)
{
	assert(channel != NULL);
	poller_->upDateChannel(channel);
}
