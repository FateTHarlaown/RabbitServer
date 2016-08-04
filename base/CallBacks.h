#ifndef RABBIT_CALLBACKS
#define RABBIT_CALLBACKS

#include <boost/function.hpp>
#include "../net/NetAddr.h"
namespace Rabbit{
namespace net{
class TcpConnection;
typedef boost::shared_ptr<TcpConnection> ConnectionPtr;	
//channel event callback function
typedef boost::function<void ()> EventCallbackFunction;
//timer callback function
typedef boost::function<void ()> TimerCallBack;
//typefef  callback function for EventLoop QueueInLoop
typedef boost::function<void ()> Functors;
//callback function for TcpServer and Acceptor
typedef boost::function<void (ConnectionPtr)> ConnectionCallback;
typedef boost::function<void (ConnectionPtr, char * buffer, ssize_t len)> MessageCallback;
//Acceptor newConnection callback function
typedef boost::function<void (int sockfd, Rabbit::net::NetAddr peerAddr)> NewConnectionCallback;
}
}

#endif
