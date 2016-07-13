#ifndef RABBIT_NET_ACCEPTOR
#define RABBIT_NET_ACCEPTOR

#include <boost/noncopyable.hpp>
#include "Channel.h"
#include "EventLoop.h"
#include "NetAddr.h"
#include "Socket.h"
#include "sys/types.h"

namespace Rabbit{
namespace net{

class Acceptor:boost::noncopyable
{
public:
	Acceptor(EventLoop * loop, const char * ip, uint16_t port); 
	void registAcceptor();
	
private:
	void handleRead();//this funtion is to binded to the acceptChannel
	EventLoop * loop_;
	NetAddr addr_;
	Socket sock_;
	Channel accepChannel_;
};
}
}

#endif
