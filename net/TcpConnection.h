#ifndef RABBIT_NET_TCPCONNECTION_H
#define RABBIT_NET_TCPCONNECTION_H

#include <string>
#include <boost/scoped_ptr.hpp>
#include <boost/atomic.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "../base/CallBacks.h"

namespace Rabbit{
namespace net{

class EventLoop;
class NetAddr;
class Channel;
class Socket;
class TcpConnection:boost::noncopyable,
					public boost::enable_shared_from_this<TcpConnection>
{
public:
	TcpConnection(EventLoop * loop, const std::string & name, int sockfd, const NetAddr & local, const NetAddr & peer);
	void setMessageCallback(MessageCallback & func);
	void setConnectionCallback(ConnectionCallback & func);
	void connectionEstablish();
private:
	void handleRead();
	enum State {kConnecting, kConnected};
	EventLoop * loop_;
	std::string name_;
	boost::atomic<State> state_;	
	boost::scoped_ptr<Channel> channel_;
	boost::scoped_ptr<Socket> sock_;
	NetAddr localAddr;
	NetAddr peerAddr;
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
};
}
}

#endif
