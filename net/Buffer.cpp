#include "Buffer.h"

using namespace Rabbit;
using namespace Rabbit::net;
#include <assert.h>

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
