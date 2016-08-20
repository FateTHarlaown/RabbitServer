#include <signal.h>

namespace Rabbit{
namespace net{

class IgnoreSigPipe
{
public:
	IgnoreSigPipe()
	{
		signal(SIGPIPE, SIG_IGN);
	}

};
}
}
