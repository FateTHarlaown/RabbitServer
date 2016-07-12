#ifndef RABBIT_NET_NETADDR
#define RABBIT_NET_NETADDR

#include <netinet/in.h>
#include <sys/types.h>

namespace Rabbit{
namespace net{

/* ******************************************
 *this class package the address of ip and port
 *the family of it will always be AF_INET
 * ********************************************/
class NetAddr
{
public:
	NetAddr(const char * ip, uint16_t port);

		uint16_t getPort()
		{
			return addr_.sin_port;
		}

		struct in_addr getInAddr()
		{
			return addr_.sin_addr;
		}

		bool isValid();
	
private:
	struct sockaddr_in addr_;
	int ret;
};

}
}

#endif
