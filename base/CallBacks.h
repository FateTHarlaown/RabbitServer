#ifndef RABBIT_CALLBACKS
#define RABBIT_CALLBACKS

#include <boost/function.hpp>

namespace Rabbit{
namespace net{
//channel event callback function
typedef boost::function<void ()> EventCallbackFunction;
//timer callback function
typedef boost::function<void ()> TimerCallBack;

}
}

#endif
