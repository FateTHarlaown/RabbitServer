#ifndef RABBIT_NET_CHANNEL_H
#define RABBIT_NET_CHANNEL_H

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/weak_ptr.hpp>

namespace Rabbit{
namespace net{

class EventLoop;
class Channel:boost::noncopyable
{
public:
	typedef boost::function<void ()> EventCallbackFunction;
	Channel(EventLoop * loop, int fd);

	void update();
	void remove();

	void setReadCallBack(const EventCallbackFunction & func)
	{
		readCallback = func;
	}

	void setWriteCallBack(const EventCallbackFunction & func)
	{
		writeCallback = func;
	}

	void setErrorCallBack(const EventCallbackFunction & func)
	{
		errorCallback = func;
	}

	void setCloseCallBack(const EventCallbackFunction & func)
	{
		closeCallback = func;
	}

	void handleEvent();
	void enableReading()
	{
		events_ |= ReadEvent;
	}
	void enableWriting()
	{
		events_ |= WriteEvent;
	}
	void disableWriting()
	{
		events_ &= ~WriteEvent;
	}
	void disableReading()
	{
		events_ &= ~ReadEvent;
	}
	void disableAll()
	{
		events_ = NoEvent;
	}
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

	int fd()
	{
		return fd_;
	}

	int index()
	{
		return index_;
	}

	EventLoop * ownerLoop()
	{
		return loop_;
	}

	void tie(const boost::shared_ptr<void> & obj)
	{
		tied_ = true;
		tie_ = obj;
	}


private:
	EventLoop * loop_;
	int fd_;
	int index_;
	int events_;
	int revents_;
	//channel's owner is tied to it, ensure the owner not die when run EventHandle
	bool tied_;
	boost::weak_ptr<void> tie_;
	/*three call back funtion for three kind of events*/
	EventCallbackFunction readCallback;
	EventCallbackFunction writeCallback;
	EventCallbackFunction errorCallback;
	EventCallbackFunction closeCallback;
	void handleEventWithGuard();
	const static int NoEvent;
	const static int ReadEvent;
	const static int WriteEvent;
};

}
}
#endif

