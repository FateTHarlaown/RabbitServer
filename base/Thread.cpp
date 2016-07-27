#include "Thread.h"
#include "Gettid.h"
#include <assert.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace Rabbit;
using namespace Rabbit::base;

namespace Rabbit
{
namespace CurrentThread
{
	__thread int t_cachedTid = 0;
	__thread char t_tidString[32];
	__thread int t_tidStringLength = 6;
	__thread const char* t_threadName = "unknown";
}

namespace detail{

struct ThreadData
{
	typedef Thread::ThreadFunc ThreadFunc;
	ThreadFunc func_;
	std::string name_;
	boost::weak_ptr<pid_t> wkTid_;

	ThreadData(const ThreadFunc& func, const std::string& name, const boost::shared_ptr<pid_t>& tid): func_(func), name_(name),wkTid_(tid)
{ 
}

	void runInThread()
	{
		pid_t tid =	gettid();

		boost::shared_ptr<pid_t> ptid = wkTid_.lock();
		if (ptid)
		{
			*ptid = tid;
			ptid.reset();
		}

		Rabbit::CurrentThread::t_threadName = name_.empty() ? "RabbitThread" : name_.c_str();
		try
		{
			func_();
			Rabbit::CurrentThread::t_threadName = "finished";
		}
		catch (...)
		{
			Rabbit::CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "some exception caught in Thread %s\n", name_.c_str());
			abort();
		}
	}
};

//used for pthread_create as thread func
void * startThread(void * arg)
{
	ThreadData * data = static_cast<ThreadData *>(arg);
	data->runInThread();
	delete data;
	return NULL;
}

}
}


Thread::Thread(const ThreadFunc & func, const std::string& n)
  : started_(false),
		joined_(false),
		pthreadId_(0),
		tid_(new pid_t(0)),
		func_(std::move(func)),
		name_(n)
{
	setDefaultName();
}

Thread::~Thread()
{
	if(started_ && !joined_)
	{
		pthread_detach(pthreadId_);
	}
}

void Thread::setDefaultName()
{
	int num = ++numCreated_;
	if (name_.empty())
	{
		char buf[32];
		snprintf(buf, sizeof buf, "Thread%d", num);
		name_ = buf;
  }
}

void Thread::start()
{
	assert(!started_);
	started_ = true;
	// FIXME: move(func_)
	detail::ThreadData* data = new detail::ThreadData(func_, name_, tid_);
	if (pthread_create(&pthreadId_, NULL, &detail::startThread, data))
	{
    started_ = false;
		delete data; 
		fprintf(stderr, "create thread failed\n");
		abort();
	}
}

int Thread::join()
{
	assert(started_);
	assert(!joined_);
	joined_ = true;
	return pthread_join(pthreadId_, NULL);
}
