#ifndef RABBIT_BASE_THREAD_H
#define RABBIT_BASE_THREAD_H

#include <boost/atomic.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>
#include <string>

namespace Rabbit{
namespace base{

class Thread : boost::noncopyable
{
public:
	typedef boost::function<void ()> ThreadFunc;

	Thread(const ThreadFunc&, const std::string& name = std::string());
	~Thread();
	void start();
	int join(); // return pthread_join()

	bool started() const { return started_; }
	// pthread_t pthreadId() const { return pthreadId_; }
	pid_t tid() const { return *tid_; }
	const std::string& name() const { return name_; }

	static int numCreated() { return numCreated_; };

private:
	void setDefaultName();

	bool       started_;
	bool       joined_;
	pthread_t  pthreadId_;
	boost::shared_ptr<pid_t> tid_;
	ThreadFunc func_;
	std::string     name_;

	static boost::atomic_int numCreated_;
};

}
}
#endif
