#include "Buffer.h"
#include <assert.h>
#include <sys/uio.h>

using namespace Rabbit;
using namespace Rabbit::net;

static char extrabuf[65536];

Buffer::Buffer(int initSize):readIndex_(0), writeIndex_(0)
{
	if(initSize < DEFAULTINITSIZE)
		initSize = DEFAULTINITSIZE;
	buffer_.resize(initSize);
}

const char * Buffer::peek() const
{
	return begin()+readIndex_;
}

void Buffer::ensureWritebale(ssize_t len)
{
	ssize_t writeAble = writeableBytes();
	if(len > writeAble)
	{
		if(len-writeAble < readIndex_)
		{
			std::copy(begin()+readIndex_, begin()+writeIndex_, begin());
			writeIndex_ = writeIndex_ - readIndex_;
			readIndex_ = 0;
		}
	}
	else
	{
		buffer_.resize(writeIndex_+len);
	}
}

void Buffer::append(const char * source, int len)
{
	assert(source != NULL);

	if(writeableBytes() >= len)
	{
		std::copy(source, source+len, begin() + writeIndex_); 	
	}
}

void Buffer::append(const std::string & str)
{
	append(str.c_str(), str.size());
}

int Buffer::retrieve(int len)
{
	if(len >= writeIndex_ - readIndex_)
	{
		len =  writeIndex_ - readIndex_;
		writeIndex_ = readIndex_ = 0;	
	}
	else
		readIndex_ += len;
	
	return len;
}

int Buffer::retrieveAll()
{
	return retrieve(writeIndex_ - readIndex_);	
}

std::string Buffer::retrieveAsString(int len)
{
	if(len >= writeIndex_ - readIndex_)
		len = writeIndex_ - readIndex_;
	std::string tmp(begin()+readIndex_, len-1);
	writeIndex_ = readIndex_ = 0;
	return tmp;
}

std::string Buffer::retrieveAllAsString()
{
	std::string tmp(begin()+readIndex_, writeIndex_-readIndex_-1);
	writeIndex_ = readIndex_ = 0;
	return tmp;
}

int Buffer::readbleBytes()
{
	return writeIndex_ - readIndex_;
}

ssize_t Buffer::readFd(int fd, int * savedErrno)
{
	struct iovec vec[2];
	const ssize_t writeAble = writeableBytes();
	vec[0].iov_base = begin()+writeIndex_;
	vec[0].iov_len = writeAble;
	vec[1].iov_base = extrabuf;
	vec[1].iov_len = sizeof extrabuf;
	const ssize_t n = readv(fd, vec, 2);
	if(n < 0)
	{
		*savedErrno = errno;
	}
	else if(n <= writeAble)
	{
		writeIndex_ += n;
	}
	else
	{
		writeIndex_ = buffer_.size();
		append(extrabuf, n-writeAble);
	}

	return n;
}
