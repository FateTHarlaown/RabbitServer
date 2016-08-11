#include "TcpServer.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include <boost/bind.hpp>
#include <stdio.h>
#include <sys/socket.h>

using namespace Rabbit;
using namespace Rabbit::net;

namespace Rabbit{
namespace net{
	typedef boost::shared_ptr<TcpConnection> ConnectionPtr;	
	void defaultConnectionCallback(const ConnectionPtr & conn)
	{
		printf("New connection establish\n");		
	}

	void defaultMessageCallback(const ConnectionPtr & conn, Buffer * buffer, Timestamp receiveTime)
	{
	
	}
}
}

TcpServer::TcpServer(EventLoop * loop, NetAddr addr, std::string name)
					: loop_(loop),
					  name_(name),
					  acceptor_(new Acceptor(loop, addr)),
					  loopsThreadPool_(new EventLoopThreadPool(loop_, name_)),
					  nextConnectionId_(0),
					  connectionCallback_(defaultConnectionCallback),
					  messageCallback_(defaultMessageCallback)
{
	
}

TcpServer::~TcpServer()
{

}

void TcpServer::setConnectionCallback(const ConnectionCallback & func)
{
	connectionCallback_ = func;
}

void TcpServer::setMessageCallback(const MessageCallback & func)
{
	messageCallback_ = func;
}

void TcpServer::newConnetion(int fd, const NetAddr  peer)
{
	loop_->assertInLoopThread();//must be called by acceptor in IO thread
	char buf[32];
	snprintf(buf, sizeof buf, "#%d", nextConnectionId_++);
	std::string conName = name_ + buf;
	NetAddr local = NetAddr::getLocalAddr(fd);
	EventLoop * ioLoop = loopsThreadPool_->getNextLoop();
	ConnectionPtr conn(new TcpConnection(ioLoop, conName, fd, local, peer)); 
	printf("now, get a new connection: %s", conName.c_str());
	conn->setMessageCallback(messageCallback_);
	conn->setConnectionCallback(connectionCallback_);
	conn->setCloseCallBack(boost::bind(&TcpServer::removeConnection, this, _1));
	connections_[conName] = conn;
	conn->connectionEstablish();
}

void TcpServer::removeConnection(const ConnectionPtr & conn)
{
	//this function will be called by TcpConnection in it's ioLoop, but we should remove it's connectionPtr in base loop
	loop_->RunInLoop(boost::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const ConnectionPtr & conn)
{
	loop_->assertInLoopThread();	
	printf("recieved destroy\n");
	std::map<std::string, ConnectionPtr>::iterator it = connections_.find(conn->name());
	assert(it != connections_.end());
	connections_.erase(it);
	printf("pass destroy\n");
	EventLoop * ioLoop = conn->getLoop();
	ioLoop->QueueInLoop(boost::bind(&TcpConnection::connectionDestroyed, conn));				}

void TcpServer::start()
{
	acceptor_->setNewConnectionCallback(boost::bind(&TcpServer::newConnetion, this, _1, _2)); 
	acceptor_->registAcceptor();
	loopsThreadPool_->start();
}

void TcpServer::setThreadNum(size_t n)
{
	loopsThreadPool_->setThreadNum(n);
}
