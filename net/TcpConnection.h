#ifndef RABBIT_NET_TCPCONNECTION_H
#define RABBIT_NET_TCPCONNECTION_H

#include <string>
#include <boost/scoped_ptr.hpp>
#include <boost/atomic.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/any.hpp>
#include "Buffer.h"
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
	EventLoop * getLoop()
	{
		return loop_;
	}
	void setMessageCallback(const MessageCallback & func);
	void setConnectionCallback(const ConnectionCallback & func);
	void setCloseCallBack(const CloseCallback & func);
	void connectionEstablish();
	void connectionDestroyed();
	void closeConnection();
	Buffer * inBuffer()
	{
		return &inBuffer_;
	}
	Buffer * outBuffer()
	{
		return &outBuffer_;
	}
	void setContext(const boost::any & context)
	{
		context_ = context;
	}
	const boost::any & getContext() const
	{
		return context_;
	}
	boost::any * getMutableContext()
	{
		return &context_;
	}
	std::string name()
	{
		return name_;
	}
	void send(const std::string & message);
	void send(const char * data, int len);

private:
	void handleRead();
	void handleClose();
	void handleWrite();
	void handleError();
	void closeConnectionInLoop();
	void sendInLoop(const std::string & message);
	void sendInLoop(const char * data, int len);

	enum State {kConnecting, kConnected, kDisconnected, kDisconnecting};
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
	Buffer inBuffer_;
	Buffer outBuffer_;
	boost::any context_;
};
}
}

#endif
