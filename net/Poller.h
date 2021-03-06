#ifndef RABBIT_NET_POLLER_H
#define  RABBIT_NET_POLLER_H

#include <sys/epoll.h>
#include <vector>
#include <boost/noncopyable.hpp>

namespace Rabbit{
namespace net{
class EventLoop;
class Channel;
class Poller:boost::noncopyable
{
public:	
	typedef std::vector<struct epoll_event>	EventList;
	typedef std::vector<Channel*> ChannelList;
	Poller(EventLoop * loop);
	void Poll(int timeOutMs, ChannelList * activeChannels);
	void upDateChannel(Channel * channel);
	void removeChannel(Channel * channel);
private:
	void fillActiveChannels(int num, ChannelList * activeChannels);
	int epollfd_;
	EventLoop * ownerLoop_;
	EventList events_;
	static const int AddOperation;
	static const int DelOperation;
	static const int ModOperation;
	static const int InitEventsNum = 255;
};
}
}
#endif
