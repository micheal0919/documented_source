#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "../common/Sockets.h"
#include <string>

class Client
{
public:
	Client();
	~Client();

	bool Client::init(const std::string& file_name,
		const std::string& svr_addr, int svr_port);

	bool start();
private:
	bool send(const std::string& buf, bool isEnd);
private:
	void *m_context;
	void *m_requester;
	std::string m_addr;
	int m_port;
	std::string m_svr_addr;
	int m_svr_port;

	std::string m_file_name;
};

#endif // __CLIENT_H__
