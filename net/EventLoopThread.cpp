#include "EventLoopThread.h"
#include <boost/bind.hpp>
#include "EventLoop.h"

using namespace Rabbit;
using namespace Rabbit::net;
using namespace Rabbit::base;

EventLoopThread::EventLoopThread(const std::string& name) : loop_(NULL), 
	                            exiting_(false),
								thread_(boost::bind(&EventLoopThread::threadFunc, this), name),
								mutex_(),
								cond_(mutex_)
{
}

EventLoopThread::~EventLoopThread()
{
	exiting_ = true;
	stopLoop();
	thread_.join();
}

void EventLoopThread::stopLoop()
{
	{
		MutexLockGuard lock(mutex_);
		while(loop_ != NULL)
		{
			loop_->stop();
			cond_.wait();
		}
	}
}

EventLoop* EventLoopThread::startLoop()
{
  assert(!thread_.started());
  thread_.start();

  {
    MutexLockGuard lock(mutex_);
    while (loop_ == NULL)
    {
      cond_.wait();
    }
  }

  return loop_;
}

void EventLoopThread::threadFunc()
{
  EventLoop loop;
  {
    MutexLockGuard lock(mutex_);
    loop_ = &loop;
    cond_.notify();
  }
  loop.loop();
  {
	MutexLockGuard lock(mutex_);
	loop_ = NULL;
	cond_.notify();
  }
}
