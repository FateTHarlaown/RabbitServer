#ifndef RABBIT_BASE_GETTID
#define RABBIT_BASE_GETTID

#include <sys/syscall.h>
namespace Rabbit{
namespace base{
	#define gettid() syscall(__NR_gettid)
}
}

#endif
