#include "NetAddr.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

using namespace Rabbit;
using namespace Rabbit::net;

NetAddr::NetAddr(const char * ip, uint16_t port):ret(0)
{
	bzero(&addr_, sizeof(addr_));
	addr_.sin_family = AF_INET;
	ret = inet_aton(ip, &addr_.sin_addr);
	addr_.sin_port = htons(port);	
}

NetAddr::NetAddr(const struct sockaddr_in addr):ret(0)
{
	addr_ = addr;
}

//this function return ture if the ip is valid
bool NetAddr::isValid()
{
	if(ret == 0)
		return true;
	else
		return false;
}

NetAddr NetAddr::getLocalAddr(int fd)
{
	struct sockaddr_in local;
	int ret;
	int sock_len = sizeof local;
	ret = getsockname(fd, (struct sockaddr *)&local,(socklen_t*) &sock_len);
	if(ret != 0)
	{
		fprintf(stderr, "Failed to get the Socket info fo fd:%d", fd);
		NetAddr tmp("0.0.0.0", 0);
		return tmp;
	}
	NetAddr tmp(local);
	return tmp;
}

NetAddr NetAddr::getPeerAddr(int fd)
{
	struct sockaddr_in peer;
	int ret;
	int sock_len = sizeof peer;
	ret = getpeername(fd, (struct sockaddr *)&peer,(socklen_t*) &sock_len);
	if(ret != 0)
	{
		fprintf(stderr, "Failed to get the peer info fo fd:%d", fd);
		NetAddr tmp("0.0.0.0", 0);
		return tmp;
	}
	NetAddr tmp(peer);
	return tmp;
}
