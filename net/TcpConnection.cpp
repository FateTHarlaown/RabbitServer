#include <string>
#include <stdio.h>
#include <boost/bind.hpp>
#include <errno.h>
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

TcpConnection::~TcpConnection()
{
	printf("des connection %s\n", name_.c_str());
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
	int savedErrno = 0;
	int  n = inBuffer_.readFd(channel_->fd(), &savedErrno);
	if(n > 0)
		messageCallback_(shared_from_this(), &inBuffer_, Timestamp::now());
	else if(n == 0)
	{
		handleClose();
	}
	else
	{
		errno = savedErrno;
		perror("read failed!");
		handleError();
	}
}

void TcpConnection::connectionEstablish()
{
	//now just test the reading function
	channel_->setReadCallBack(boost::bind(&TcpConnection::handleRead, this));
	channel_->setCloseCallBack(boost::bind(&TcpConnection::handleClose, this));
	channel_->setErrorCallBack(boost::bind(&TcpConnection::handleError, this));	
	channel_->setWriteCallBack(boost::bind(&TcpConnection::handleWrite,this));
	channel_->enableReading();
	channel_->tie(shared_from_this());
	state_ = kConnected;
	connectionCallback_(shared_from_this());
}

void TcpConnection::handleClose()
{
	printf("to close connection %s\n", name_.c_str());
	assert(state_ == kConnected || state_ == kDisconnecting);
	state_ = kDisconnected;
	loop_->assertInLoopThread();	
	channel_->disableAll();
	closeCallback_(shared_from_this());
}

//run in depending functors
void TcpConnection::connectionDestroyed()
{
	
	printf("remove channel for %s\n", name_.c_str());
	if(state_ == kConnected || state_ == kDisconnecting)
	{
		state_ = kDisconnected;
		channel_->disableAll();
	}
	channel_->remove();
}

void TcpConnection::handleError() 
{
	fprintf(stderr, "TcpConnection error\n");
}

void TcpConnection::closeConnection()
{
	if(state_ == kConnected || state_ == kConnecting)
	{
		loop_->QueueInLoop(boost::bind(&TcpConnection::closeConnectionInLoop, shared_from_this()));
	}
}
	
void TcpConnection::closeConnectionInLoop()
{
	loop_->assertInLoopThread();
	if(state_ == kConnected || state_ == kConnecting)
	{
		if(!channel_->isWriting())
		{
			handleClose();
		}
		else
		{
			state_ = kDisconnecting;
		}
	}
}

void TcpConnection::send(const std::string & message)
{
	send(message.data(), message.size());
}

void TcpConnection::send(const char * data, int len)
{
	if(state_ == kConnected)
	{
		if(loop_->isInLoopThread())
		{
			sendInLoop(data, len);
		}
		else
		{
			loop_->RunInLoop(boost::bind(&TcpConnection::sendInLoop, shared_from_this(), data, len));
		}
	}
}

void TcpConnection::sendInLoop(const char * data, int len)
{
	loop_->assertInLoopThread();
	int wroted = 0;
	//for test
	//outBuffer_.append("moximoxi", 8);
	//channel_->enableWriting();
	if(!channel_->isWriting() && outBuffer_.readbleBytes() == 0)
	{
		wroted = write(channel_->fd(), data, len);	
		if(wroted < 0)
		{
			wroted = 0;
			if(errno != EAGAIN)
			{
				perror("");
				fprintf(stderr, "send message failed in %s\n", name_.c_str());
			}
		}
	}
	assert(wroted >= 0);
	if(wroted < len)
	{
		printf("put rest data in outBuffer_ \n"); 
		outBuffer_.append(data+wroted, len-wroted); 
		if(!channel_->isWriting())
		{
			channel_->enableWriting();
		}
	}
}

void TcpConnection::handleWrite()
{
	loop_->assertInLoopThread();
	int wroted = 0;
	if(channel_->isWriting())
	{
		wroted = write(channel_->fd(), outBuffer_.peek(), outBuffer_.readbleBytes());
		printf("%d bytes can write\n", outBuffer_.readbleBytes());
		if(wroted > 0)
		{
			outBuffer_.retrieve(wroted);
			if(outBuffer_.readbleBytes() == 0)
			{
				channel_->disableWriting();
				if(state_ == kDisconnecting)
				{
					handleClose();
				}
			}
		}
		else
		{
			if(errno != EAGAIN)
			{
				printf("writed %d bytes in buffr, ", wroted); 
				perror("");
				printf("handleWrite send message failed in %s\n", name_.c_str());
			}
		}
	}
}
