#include "../base/Mutex.h"
#include "../base/Condtion.h"
#include "../base/Thread.h"
#include <string>
namespace Rabbit
{
namespace net
{

class EventLoop;
class EventLoopThread : boost::noncopyable
{
public:
    EventLoopThread(const std::string& name = std::string());
    ~EventLoopThread();
    EventLoop* startLoop();

private:
	void stopLoop();
    void threadFunc();
    EventLoop* loop_;
    bool exiting_;
	Rabbit::base::Thread thread_;
	Rabbit::base::MutexLock mutex_;
	Rabbit::base::Condition cond_;
};

}
}
