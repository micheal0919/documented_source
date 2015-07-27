#ifndef __SERVER_H__
#define __SERVER_H__

#include "../Common/RCMMessage.pb.h"
#include <string>
#include <list>
#include <map>

class Server
{
public:
	Server();
	~Server();

	bool init(int port);
	bool start();

private:
	bool handleInput(const char* buf, int len);
	bool handleUpload(const RCMMessage::Message& msg);
	bool handleList(const RCMMessage::Message& msg);
	bool handleDownload(const RCMMessage::Message& msg);
	bool handleFileSearch(const RCMMessage::Message& msg);
	bool handleStringSearch(const RCMMessage::Message& msg);
private:
	void *m_context;
	void *m_responder;
//	std::map<std::string, std::list<std::string> > m_catalog;
	std::list<std::string> m_catalog;
};

#endif // __SERVER_H__
