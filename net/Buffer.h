#ifndef RABBIT_NET_BUFFER
#define RABBIT_NET_BUFFER

#include <vector>
#include <string>

namespace Rabbit{
namespace net{
class Buffer
{
public:
	Buffer(int initSize = DEFAULTINITSIZE); 
	//return the the pionter point to the start of the data 
	const char * peek() const;
	void append(const char * source, int len);
	void append(const std::string & str);
	//retrieve some data which has been apended to buffer
	//return how many data are been retieved actualy
	int retrieve(int len);
	int retrieveAll();
	std::string retrieveAsString(int len);
	std::string retrieveAllAsString();
	//how many data in the buffer now
	int readbleBytes();
	ssize_t readFd(int fd, int * savedErrno);
private:
	const char * begin() const
	{
		return &(*buffer_.begin());
	}
	
	char * begin()
	{
		return &(*buffer_.begin());
	}

	int writeableBytes()
	{
		return buffer_.capacity()-writeIndex_;
	}
	void ensureWritebale(ssize_t len);

	std::vector<char> buffer_;
	int readIndex_;
	int writeIndex_;
	static const int DEFAULTINITSIZE = 1024;
};
}
}

#endif
