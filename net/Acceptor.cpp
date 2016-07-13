#include "Accepter.h"
#include <boost/bind.hpp>

using namespace Rabbit;
using namespace Rabbit::net;

Acceptor::Acceptor(EventLoop * loop, const char * ip, uint16_t port):loop_(loop), addr_(ip, port) ,sock_(), accepChannel_(loop, sock_.fd())
{
	sock_.setNodelay(true);
	sock_.setReuseport(true);
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
	//now will not accept new connection , just for test;
    int confd = accept(sock_.fd(), NULL, NULL);
	sleep(2);
	printf("I accep a client %d\n", confd);
}
