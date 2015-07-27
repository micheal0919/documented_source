#ifndef __SERVER_H__
#define __SERVER_H__

#include "../common/Sockets.h"
#include <string>

class Server
{
public:
	Server();
	~Server();

	void init(int port);
	bool run();
private:
	bool handle(const std::string& buf);
private:
	std::string m_addr;
	int m_port;
	void *m_context;
	void *m_responder;
};

/*
class ServerBase : public processing
{
public:
	ServerBase(int port);
	virtual ~ServerBase();
	
	virtual void run();
private:
	SocketListener m_listener;
	bool m_is_running;
};

class Server
{
public:
	Server(int port);
	~Server();

	bool start();
	bool stop();


private:
	ServerBase m_base;
	bool m_is_running;
};
*/
#endif // __SERVER_H__
