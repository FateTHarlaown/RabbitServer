#ifndef RABBIT_NET_SOCKET
#define RABBIT_NET_SOCKET

#include <boost/noncopyable.hpp>
#include <sys/socket.h>
#include "NetAddr.h"

namespace Rabbit {
namespace net{

class Socket:boost::noncopyable
{
public:
	Socket();
	~Socket();

	int fd()
	{
		return fd_;
	}
	int bindAddr(NetAddr addr);
	int setNodelay(bool op);
	int setReuseAddr(bool op);
	int setReuseport(bool op);
	int startListen();
	int connectTo(NetAddr remoteAddr);
	static const int BACKLOG = 20;

private:
	int fd_;	
};
}
}

#endif
