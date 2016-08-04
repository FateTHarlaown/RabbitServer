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
	NetAddr(const struct sockaddr_in addr);

		uint16_t getPort()
		{
			return addr_.sin_port;
		}

		struct in_addr getInAddr()
		{
			return addr_.sin_addr;
		}

		bool isValid();

		struct sockaddr_in  getAddr()
		{
			return addr_;
		}
		//get a fd's local address, if failed we will get a NetAddr("0.0.0.0", 0")
		static NetAddr getLocalAddr(int fd);
		//get a fd's peer address, if failed we will get a NetAddr("0.0.0.0", 0")
		static NetAddr getPeerAddr(int fd);
	
private:
	struct sockaddr_in addr_;
	int ret;
};

}
}

#endif
