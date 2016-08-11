#ifndef RABBIT_NET_TCPSERVER_H
#define RABBIT_NET_TCPSERVER_H

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "NetAddr.h"
#include "Acceptor.h"
#include "EventLoopThreadPool.h"
#include <string>
#include <map>
#include "../base/CallBacks.h" 

namespace Rabbit{
namespace net{

class EventLoop;
class TcpConnection;
class TcpServer:boost::noncopyable
{
public:
	TcpServer(EventLoop * loop, NetAddr addr, std::string name = "Tcp"); 
	~TcpServer();
	void setConnectionCallback(const ConnectionCallback & func);
	void setMessageCallback(const MessageCallback & func);
	void start(); 
	void setThreadNum(size_t n);
private: 
	void newConnetion(int fd, const NetAddr peer);
	void removeConnection(const ConnectionPtr & conn);
	void removeConnectionInLoop(const ConnectionPtr & conn);
	EventLoop * loop_; 
	std::string name_;
	boost::scoped_ptr<Acceptor> acceptor_;	
	boost::scoped_ptr<EventLoopThreadPool> loopsThreadPool_;
	std::map<std::string, ConnectionPtr> connections_;	
	int nextConnectionId_;//aways in loop thread, no need to use atomic
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;

};
}
}

#endif	
