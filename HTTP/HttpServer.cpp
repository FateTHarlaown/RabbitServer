#include "HttpServer.h"
#include "HttpContext.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "../net/TcpConnection.h"
#include "stdio.h"
#include <boost/bind.hpp>

using namespace Rabbit;
using namespace Rabbit::net;


void defaultHttpCallback(const HttpRequest&, HttpResponse* resp)
{
	resp->setStatusCode(HttpResponse::k404NotFound);
	resp->setStatusMessage("Not Found");
	resp->setCloseConnection(true);
}


HttpServer::HttpServer(EventLoop* loop,
                       const NetAddr & listenAddr,
                       const std::string & name)
  : server_(loop, listenAddr, name),
    httpCallback_(defaultHttpCallback)
{
	server_.setConnectionCallback(boost::bind(&HttpServer::onConnection, this, _1));
	server_.setMessageCallback(boost::bind(&HttpServer::onMessage, this, _1, _2, _3));
}

HttpServer::~HttpServer()
{
}

void HttpServer::start()
{
	server_.start();
}

void HttpServer::onConnection(const ConnectionPtr& conn)
{
	if (conn->establlished())
	{
		conn->setContext(HttpContext());
	}
	printf("a cline connect by %s\n", conn->name().c_str()); 
} 

void HttpServer::onMessage(const ConnectionPtr& conn, Buffer* buf, Timestamp receiveTime)
{
	HttpContext* context = boost::any_cast<HttpContext>(conn->getMutableContext());
	const char * spaceLine = buf->peek() + buf->readbleBytes() - 4 ;
	for(int i = 0; i < 4; i++)
	{
		printf("%d\n", *(spaceLine+i));
	}
	if(strncmp(spaceLine, "\r\n\r\n", 4) == 0)
	{
		printf("now can parse request\n");
		if (!context->parseRequest(buf, receiveTime))
		{
			conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
			conn->closeConnection();
		}

		if (context->gotAll())
		{
			onRequest(conn, context->request());
			context->reset();
		}
	}
	else
	{
		printf("receive not compelte request!\n");
	}
}

void HttpServer::onRequest(const ConnectionPtr& conn, const HttpRequest& req)
{
	const std::string& connection = req.getHeader("Connection");
	bool close = connection == "close" ||
    (req.getVersion() == HttpRequest::kHttp10 && connection != "Keep-Alive");
	HttpResponse response(close);
	httpCallback_(req, &response);
	Buffer buf;
	response.appendToBuffer(&buf);
	conn->send(buf.peek(), buf.readbleBytes());
	if (response.closeConnection())
	{
		conn->closeConnection();
	}
}

