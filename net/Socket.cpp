#include "Socket.h"
#include <errno.h>
#include <netinet/tcp.h>
#include <stdio.h>
using namespace Rabbit;
using namespace Rabbit::net;

Socket::Socket():fd_(0)
{
	fd_ = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
}

Socket::Socket(int fd) : fd_(fd)
{

}

Socket::~Socket()
{
	close(fd_);
}

int Socket::bindAddr(NetAddr addr)
{
	int errno_saved = errno, ret;
	struct sockaddr_in bindAddr  = addr.getAddr();
	if((ret = bind(fd_, (struct sockaddr*)&bindAddr, sizeof bindAddr)) != 0)
	{
		perror("socket bind failed!");
		errno = errno_saved;
	}

	return ret;
}

int Socket::startListen()
{
	int errno_saved = errno, ret;
	ret = listen(fd_, BACKLOG);
	if(ret != 0)
	{
		perror("socket bind failed!");
		errno = errno_saved;
	}

	return ret;
}

int Socket::setNodelay(bool op)
{
	int errno_saved = errno, ret;
	int nodelay;
	if(op == true)
		nodelay = 1;
	else
		nodelay = 0;
	ret = setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof nodelay); 
	if(ret != 0)
	{
		perror("set nodelay failed");
		errno = errno_saved;
	}

	return ret;
}


int Socket::setReuseAddr(bool op)
{
	int errno_saved = errno, ret;
	int reuseAddr;
	if(op == true)
		reuseAddr = 1;
	else
		reuseAddr = 0;
	ret = setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof reuseAddr); 
	if(ret != 0) {
		perror("set reuse addr failed!");
		errno = errno_saved;
	}

	return ret;
}

int Socket::setReusePort(bool op)
{
	int errno_saved = errno, ret;
	int reusePort;
	if(op == true)
		reusePort = 1;
	else
		reusePort = 0;
	ret = setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &reusePort, sizeof reusePort); 
	if(ret != 0)
	{
		perror("set reusePort failed");
		errno = errno_saved;
	}

	return ret;
}

int Socket::connectTo(NetAddr addr)
{
	int errno_saved = errno, ret;
	struct sockaddr_in connectAddr  = addr.getAddr();
	if((ret = connect(fd_, (struct sockaddr*)&connectAddr, sizeof connectAddr)) != 0)
	{
		perror("can not connectTo peer");
		errno = errno_saved;
	}

	return ret;
}
		
int Socket::acceptConnection(NetAddr & peer)
{
	int errno_saved = errno, ret;
	struct sockaddr_in temp;
	socklen_t len = sizeof temp;
	ret = accept(fd_, (struct sockaddr *)&temp, &len);
	if(ret == -1)
	{
		perror("acceptConnection fialed!");
		errno = errno_saved;
		return ret;
	}
	peer = NetAddr(temp);
	return ret;
}

