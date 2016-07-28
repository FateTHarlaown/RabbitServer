#ifndef RABBIT_BASE_CONDITION
#define RABBIT_BASE_CONDITION

#include "Mutex.h"
namespace Rabbit{
namespace base{
class Condition : boost::noncopyable
{
public:
    explicit Condition(MutexLock& mutex): mutex_(mutex)
    {
        assert(pthread_cond_init(&pcond_, NULL) == 0);
    }

    ~Condition()
    {
        assert(pthread_cond_destroy(&pcond_) == 0);
    }

    void wait()
    {
        MutexLock::UnassignGuard ug(mutex_);
        assert(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()) == 0);
    }

    // returns true if time out, false otherwise.
    bool waitForSeconds(double seconds);

    void notify()
    {
        assert(pthread_cond_signal(&pcond_) == 0);
    }

    void notifyAll()
    {
        assert(pthread_cond_broadcast(&pcond_) == 0);
    }

private:
    MutexLock& mutex_;
    pthread_cond_t pcond_;
};
}
}

#endif
