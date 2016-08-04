#include <string>
#include <stdio.h>
#include <boost/bind.hpp>
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

void TcpConnection::setMessageCallback(MessageCallback & func)
{
	messageCallback_ = func;
}

void TcpConnection::setConnectionCallback(ConnectionCallback & func)
{
	connectionCallback_ = func;
}

void TcpConnection::handleRead()
{
	char buffer[BUFSIZ];
	ssize_t n = read(channel_->fd(), buffer, BUFSIZ);
	messageCallback_(shared_from_this(), buffer, n);
}

void TcpConnection::connectionEstablish()
{
	//now just test the reading function
	channel_->setReadCallBack(boost::bind(&TcpConnection::handleRead, this));
	channel_->enableReading();
	channel_->update();
	connectionCallback_(shared_from_this());
	state_ = kConnected;
}

