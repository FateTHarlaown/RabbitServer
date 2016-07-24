#ifndef RABBIT_BASE_MUTEX
#define RABBIT_BASE_MUTEX

#include <pthread.h>
#include <unistd.h>
#include "Gettid.h"
#include <boost/noncopyable.hpp>
#include <assert.h>


namespace Rabbit{
namespace base{

class MutexLock : boost::noncopyable
{
public:
	MutexLock(): holder_(0)
	{
		assert(pthread_mutex_init(&mutex_, NULL) == 0);
	}

	~MutexLock()
	{
		assert(holder_ == 0);
		assert(pthread_mutex_destroy(&mutex_) == 0);
	}

	// must be called when locked, i.e. for assertion
	bool isLockedByThisThread() const
	{
		return holder_ == gettid();
	}

	void assertLocked() const
	{
		assert(isLockedByThisThread());
	}

	// internal usage

	void lock()
	{
		assert(pthread_mutex_lock(&mutex_) == 0);
		assignHolder();
	}

	void unlock()
	{
		unassignHolder();
		assert(pthread_mutex_unlock(&mutex_) == 0);
	}

	pthread_mutex_t* getPthreadMutex() /* non-const */
	{
		return &mutex_;
	}

private:
	friend class Condition;
    class UnassignGuard : boost::noncopyable
	{
	public:
		UnassignGuard(MutexLock& owner): owner_(owner)
		{
			owner_.unassignHolder();
		}

		~UnassignGuard()
		{
			owner_.assignHolder();
		}

	private:
		MutexLock& owner_;
	};

	void unassignHolder()
	{
		holder_ = 0;
	}

	void assignHolder()
	{
		holder_ = gettid();
	}
	pthread_mutex_t mutex_;
	pid_t holder_; 
};

class MutexLockGuard : boost::noncopyable
{
public:
    explicit MutexLockGuard(MutexLock& mutex): mutex_(mutex)
    {
        mutex_.lock();
    }

    ~MutexLockGuard()
    {
        mutex_.unlock();
    }

private:
    MutexLock& mutex_;
};

}
}

#endif
