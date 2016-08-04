#include "Acceptor.h"
#include <stdio.h>
#include <boost/bind.hpp>

using namespace Rabbit;
using namespace Rabbit::net;

Acceptor::Acceptor(EventLoop * loop, NetAddr addr):loop_(loop), addr_(addr) ,sock_(), accepChannel_(loop, sock_.fd())
{
	sock_.setNodelay(true);
	sock_.setReusePort(true);
	sock_.bindAddr(addr_);
}

void Acceptor::registAcceptor()
{
	sock_.startListen();
	accepChannel_.enableReading();
	accepChannel_.setReadCallBack(boost::bind(&Acceptor::handleRead, this));
	accepChannel_.update();
}

void Acceptor::handleRead()
{
	loop_->assertInLoopThread();
	NetAddr peer("0.0.0.0", 0);
	int newFd = sock_.acceptConnection(peer);	
	newConnectionCallback_(newFd, peer);
}

void Acceptor::setNewConnectionCallback(const NewConnectionCallback & func)
{
	newConnectionCallback_ = func;
}

