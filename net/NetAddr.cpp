#include "NetAddr.h"
#include <arpa/inet.h>
#include <string.h>

using namespace Rabbit;
using namespace Rabbit::net;

NetAddr::NetAddr(const char * ip, uint16_t port):ret(0)
{
	bzero(&addr_, sizeof(addr_));
	addr_.sin_family = AF_INET;
	ret = inet_aton(ip, &addr_.sin_addr);
	addr_.sin_port = htons(port);	
}

//this function return ture if the ip is valid
bool NetAddr::isValid()
{
	if(ret == 0)
		return true;
	else
		return false;
}
