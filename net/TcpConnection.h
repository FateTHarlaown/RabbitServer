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
	~TcpConnection();
	void setMessageCallback(const MessageCallback & func);
	void setConnectionCallback(const ConnectionCallback & func);
	void setCloseCallBack(const CloseCallback & func);
	void connectionEstablish();
	void connectionDestroyed();
	std::string name()
	{
		return name_;
	}
private:
	void handleRead();
	void handleClose();
	void handleWrite();
	void handleError();
	enum State {kConnecting, kConnected, kDisconnected};
	EventLoop * loop_;
	std::string name_;
	boost::atomic<State> state_;	
	boost::scoped_ptr<Channel> channel_;
	boost::scoped_ptr<Socket> sock_;
	NetAddr localAddr;
	NetAddr peerAddr;
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
	CloseCallback closeCallback_;
};
}
}

#endif
