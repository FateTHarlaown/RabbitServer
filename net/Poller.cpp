#include "Poller.h"
#include "Channel.h"
#include <stdio.h>
#include <errno.h>

using namespace Rabbit;
using namespace Rabbit::net;

static const int kNew = 0;
static const int kAdded = 1;
const int Poller::AddOperation = EPOLL_CTL_ADD;
const int Poller::DelOperation = EPOLL_CTL_DEL;
const int Poller::ModOperation = EPOLL_CTL_MOD;
Poller::Poller(EventLoop * loop):ownerLoop_(loop), events_(InitEventsNum)
{
	int epollfd = epoll_create(5);
	if(epollfd < 0)
	{
		perror("Create epoll failed!");
		exit(1);
	}
	epollfd_ = epollfd;
}

//poll the events and fill activeChannels with active events
void Poller::Poll(int timeOutMs, ChannelList * activeChannels)
{
	int ret = epoll_wait(epollfd_, &(*events_.begin()), static_cast<int>(events_.size()), timeOutMs);
	if(ret == -1)
	{
		perror("epoll wait error:");
		return;
	}
	fillActiveChannels(ret, activeChannels);
	if(static_cast<int>(events_.size()) == ret)
	{
		events_.resize(ret*2);
	}
}

void Poller::fillActiveChannels(int num, ChannelList * activeChannels)
{
	for(int i = 0; i < num; i++)
	{
		Channel * pchannel = static_cast<Channel*>(events_[i].data.ptr);
		pchannel->set_revnets(events_[i].events);
		(*activeChannels).push_back(pchannel);
	}
}

//update or add a channel to the poller
void Poller::upDateChannel(Channel * channel)
{
	int index = channel->index();
	if(index == kNew)//it's a new channel
	{
		struct epoll_event event;
		event.data.ptr = static_cast<void*>(channel);
		event.events = channel->events();
		epoll_ctl(epollfd_, AddOperation, channel->fd(), &event);
		channel->set_index(kAdded);
	}
	else if(index == kAdded)//this channe exists, update it
	{
		struct epoll_event event;
		event.events = channel->events();
		epoll_ctl(epollfd_, ModOperation, channel->fd(), &event);
	}
	else
		perror("can not understand this kind of channel!");
}

//reomve a channel from this poller
void Poller::removeChannel(Channel * channel)
{
	int index = channel->index();
	if(index == kNew)
		return;
	struct epoll_event event;
	event.data.ptr = static_cast<void*>(channel);
	event.events = channel->events();
	epoll_ctl(epollfd_, DelOperation, channel->fd(), &event);
	channel->set_index(kNew);
}
