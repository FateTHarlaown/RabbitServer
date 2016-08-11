#ifndef RABBIT_NET_EVENTTHREADPOOL_H
#define RABBIT_NET_EVENTTHREADPOOL_H
#include <vector>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace Rabbit
{

namespace net
{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : boost::noncopyable
{
    public:
        typedef boost::function<void(EventLoop*)> ThreadInitCallback;

        EventLoopThreadPool(EventLoop* baseLoop, const std::string& nameArg);
        ~EventLoopThreadPool();
        void setThreadNum(int numThreads) { numThreads_ = numThreads; }
        void start();

        // valid after calling start()
        /// round-robin
        EventLoop* getNextLoop();

        EventLoop* getLoopForHash(size_t hashCode);

        std::vector<EventLoop*> getAllLoops();

        bool started() const
        { return started_; }

        const std::string& name() const
        { return name_; }

     private:

        EventLoop* baseLoop_;
		std::string name_;
        bool started_;
        int numThreads_;
        int next_;
        boost::ptr_vector<EventLoopThread> threads_;
        std::vector<EventLoop*> loops_;//those loops are stack var int the stack of it's thread
};

}
}

#endif
