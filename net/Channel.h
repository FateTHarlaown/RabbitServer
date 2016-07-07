#ifndef RABBIT_NET_CHANNEL_H
#define RABBIT_NET_CHANNEL_H

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace Rabbit{
namespace net{

class EventLoop;
class Channel:boost::noncopyable
{
public:
	typedef boost::function<void ()> EventCallbackFunction;
	Channel(EventLoop * loop, int fd);


	void setReadCallBack(EventCallbackFunction & func)
	{
		readCallback = func;
	}

	void setWriteCallBack(EventCallbackFunction & func)
	{
		writeCallback = func;
	}

	void setErrorCallBack(EventCallbackFunction & func)
	{
		errorCallback = func;
	}

	void handleEvent();
	void enableReading()
	{
		events_ |= ReadEvent;
	}
	//void enableWriting();
	//void disableWriting();
	//void disableAll();
	void set_revnets(int revents)
	{
		revents_ = revents;
	}

	bool isNoEvent()
	{
		return events_ == NoEvent;
	}

	void set_index(int index)
	{
		index_ = index;
	}

	int events()
	{
		return events_;
	}

	int index()
	{
		return index_;
	}

	EventLoop * ownerLoop()
	{
		return loop_;
	}


private:
	EventLoop * loop_;
	int fd_;
	int index_;
	int events_;
	int revents_;
	/*three call back funtion for three kind of events*/
	EventCallbackFunction readCallback;
	EventCallbackFunction writeCallback;
	EventCallbackFunction errorCallback;
	const static int NoEvent;
	const static int ReadEvent;
	const static int WriteEvent;
};

}
}
#endif

