#include "Server.h"
#include "../common/Message.h"
#include <stdio.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <winsock2.h>
#include "../common/zmq.h"
#include "../common/zmq_utils.h"

using namespace std;

Server::Server()
:m_context(NULL),
m_responder(NULL)
{

}

Server::~Server()
{
	if (m_context)
	{
		zmq_ctx_destroy(m_context);
	}
	if (m_responder)
	{
		zmq_close(m_responder);
	}
}

void Server::init(int port)
{
	m_port = port;

	//  Socket to talk to clients
	m_context = zmq_ctx_new();
	m_responder = zmq_socket(m_context, ZMQ_REP);
	char buf[64];
	sprintf_s(buf, "tcp://*:%d", m_port);
	cout << buf << endl;
	int rc = zmq_bind(m_responder, "tcp://*:5555");
	assert(rc == 0);
}

bool Server::run()
{
	

	while (1) {
		cout << __FILE__ << ", line: " << __LINE__ << endl;
		char buffer[1024 * 4];
		memset(buffer, 0, sizeof(buffer));
		int len = zmq_recv(m_responder, buffer, 1024 * 4, 0);
		cout << __FILE__ << ", line: " << __LINE__ << endl;
//		buffer[len] = '\0';
		cout << buffer << endl;
		handle(string(buffer));
		cout << __FILE__ << ", line: " << __LINE__ << endl;
	}

	/*
	SocketListener listener(m_port);
	listener.waitForConnect();
	Socket recvr;
	while (true)
	{
		recvr = listener.waitForConnect();

		cout << "fuck you!!!!!!!!" << endl;
		cout << recvr.getRemoteIP() << endl;
		string ss = recvr.stripTerminator(recvr.recv());
		cout << ss << endl;

		Message req;
		req.parse(ss);
		cout << "The command received is " << req.getCmd() << endl;

		Message rsp;
		rsp.setCmd("response");
		rsp.setSenderAddress(req.getReceiverAddress());
		rsp.setSenderPort(req.getReceiverPort());
		rsp.setReceiverAddress(req.getSenderAddress());
		rsp.setReiverPort(req.getSenderPort());

		ss = rsp.serialize();
		recvr.send(ss);
	}*/
}

bool Server::handle(const std::string& buf)
{
	cout << __FILE__ << ", line: " << __LINE__ << endl;
	static FILE* file = NULL;
	static string file_name;

	Message req;
	req.parse(buf);
	cout << __FILE__ << ", line: " << __LINE__ << endl;
	string cmd = req.getCmd();
	cout << "cmd is " << cmd  << "aaaaaaa"<< endl;


	size_t pos1 = cmd.find_last_of(' ');
	
	char cmd_buf[128];
	memset(cmd_buf, 0, sizeof(cmd_buf));
	cmd.copy(cmd_buf, cmd.size(), pos1 + 1);
	string option = cmd_buf;
	cout << "option is " << option << endl;
	
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	if (option == "end")
	{
		fclose(file);
		file = NULL;
		cout << __FILE__ << ", line: " << __LINE__ << endl;
	}
	else if (option != "end")
	{
		pos1 = cmd.find_last_of('//');
		memset(cmd_buf, 0, sizeof(cmd_buf));
		cmd.copy(cmd_buf, cmd.size(), pos1 + 1);
		file_name = cmd_buf;
		cout << "file name is " << file_name << endl;
		cout << __FILE__ << ", line: " << __LINE__ << endl;
		if (file == NULL)
		{
			file = fopen(file_name.c_str(), "w");
			assert(file);
			cout << __FILE__ << ", line: " << __LINE__ << endl;
		}
		string content = req.getContent();
		fwrite(content.c_str(), 1, content.size(), file);
		cout << __FILE__ << ", line: " << __LINE__ << endl;
	}

	cout << __FILE__ << ", line: " << __LINE__ << endl;

	Message res;
	res.setCmd("OK");
	res.setReceiverAddress(req.getSenderAddress());
	res.setSenderAddress(req.getReceiverAddress());
	string res_str = res.serialize();
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	zmq_send(m_responder, res_str.c_str(), res_str.size(), 0);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	return true;
}

int main()
{
	int port = 5555;
	Server svr;
	svr.init(port);
	svr.run();
	return 0;
}

/*
ServerBase::ServerBase(int port)
:m_listener(port),
m_is_running(true)
{

}

ServerBase::~ServerBase()
{

}

void ServerBase::run()
{
	while (m_is_running)
	{
		Socket soc(m_listener.waitForConnect());
		string ss = soc.recv();
		Message msg;
		msg.parse(ss);
		cout << "The command received is " << msg.getCmd() << endl;
	}
	

}

Server::Server(int port)
:m_base(port)
{
}


Server::~Server()
{
}

bool Server::start()
{

}

bool Server::stop()
{

}
*/

