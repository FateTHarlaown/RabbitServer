#include <string>
#include <stdio.h>
#include <boost/bind.hpp>
#include "EventLoop.h"
#include "TcpConnection.h"
#include "Socket.h"
#include "NetAddr.h"
#include "Channel.h"

using namespace Rabbit;
using namespace Rabbit::net;


TcpConnection::TcpConnection(EventLoop * loop, const std::string &name, int sockfd, const NetAddr & local, const NetAddr & peer)
				: loop_(loop),
				  name_(name),
				  state_(kConnecting),
				  channel_(new Channel(loop_, sockfd)),
				  sock_(new Socket(sockfd)),
				  localAddr(local),
				  peerAddr(peer)
{
}

void TcpConnection::setMessageCallback(const MessageCallback & func)
{
	messageCallback_ = func;
}

void TcpConnection::setConnectionCallback(const ConnectionCallback & func)
{
	connectionCallback_ = func;
}

void TcpConnection::setCloseCallBack(const CloseCallback & func)
{
	closeCallback_ = func;	
}

void TcpConnection::handleRead()
{
	char buffer[BUFSIZ];
	ssize_t n = read(channel_->fd(), buffer, BUFSIZ);
	if(n > 0)
		messageCallback_(shared_from_this(), buffer, n);
	else if(n == 0)
	{
		handleClose();
	}
	else
	{
		handleError();
	}
}

void TcpConnection::connectionEstablish()
{
	//now just test the reading function
	channel_->setReadCallBack(boost::bind(&TcpConnection::handleRead, this));
	channel_->setCloseCallBack(boost::bind(&TcpConnection::handleClose, this));
	channel_->setErrorCallBack(boost::bind(&TcpConnection::handleError, this));	
	channel_->enableReading();
	channel_->update();
	channel_->tie(shared_from_this());
	connectionCallback_(shared_from_this());
	state_ = kConnected;
}

void TcpConnection::handleClose()
{
	loop_->assertInLoopThread();	
	state_ = kDisconnected;	
	channel_->disableAll();
	closeCallback_(shared_from_this());
}

void TcpConnection::connectionDestroyed()
{
	assert(state_ == kConnected);
	channel_->disableAll();
	channel_->remove();
}

void TcpConnection::handleError() 
{
	fprintf(stderr, "TcpConnection error\n");
}
