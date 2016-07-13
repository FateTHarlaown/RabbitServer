#ifndef RABBIT_NET_EVENT_LOOP_H
#define RABBIT_NET_EVENT_LOOP_H

#include <boost/scoped_ptr.hpp>
#include <vector>
#include <boost/noncopyable.hpp>
#include "Poller.h"

namespace Rabbit{
namespace net{
class Channel;
class EventLoop:boost::noncopyable
{
public:
	typedef std::vector<Channel*> ChannelList;
	EventLoop();
	void upDateChannel(Channel * channel);
	void loop();
	void stop();

private:
	ChannelList activeChannels;
	bool quit_;
	boost::scoped_ptr<Poller> poller_;
};

}
}

#endif
