#ifndef RABBIT_CALLBACKS
#define RABBIT_CALLBACKS

#include <boost/function.hpp>

namespace Rabbit{

//channel event callback function
typedef boost::function<void ()> EventCallbackFunction;
//timer callback function
typedef boost::function<void ()> TimerCallBack;
//typefef  callback function for EventLoop QueueInLoop
typedef boost::function<void ()> Functors;

}

#endif
