#include "Client.h"
#include "../common/Message.h"
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include "../common/zmq.h"
#include "../common/zmq_utils.h"

using namespace std;

Client::Client()
:m_context(NULL),
m_requester(NULL)
{
}


Client::~Client()
{
	if (m_context != NULL)
	{
		zmq_ctx_destroy(m_context);
		
	}
	if (m_requester != NULL)
	{
		zmq_close(m_requester);
	}
}

bool Client::init(const std::string& file_name, 
	const std::string& svr_addr, int svr_port)
{
	m_svr_addr = svr_addr;
	m_svr_port = svr_port;
	m_file_name = file_name;
	m_addr = "127.0.0.1";

	m_context = zmq_ctx_new();
	m_requester = zmq_socket(m_context, ZMQ_REQ);

	char buf[64];
	sprintf_s(buf, "tcp://localhost:%d", m_svr_port);
	cout << buf << endl;
//	int rc = zmq_connect(m_requester, buf);
	int rc = zmq_connect(m_requester, "tcp://127.0.0.1:5555");
	if (rc != 0)
	{
		cout << zmq_strerror(zmq_errno()) << endl;
	}
	assert( rc == 0 );
	return true;

}

bool Client::start()
{
	char buf[1028];
	
	FILE* file = fopen(m_file_name.c_str(), "r");
	assert(file);

	while (true)
	{
		memset(buf, 0, sizeof(buf));
		int len = fread(buf, 1, 1024, file);
		if (len == 0)
		{
			send("", true);
			cout << __FILE__ << ", line: " << __LINE__ << endl;
			break;
		}
//		buf[len] = '\0';
		cout << buf << endl;
		send(string(buf), false);
		cout << __FILE__ << ", line: " << __LINE__ << endl;
	}
	fclose(file);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	return true;
	/*
//	Socket soc(m_addr, m_port);
	Socket soc;
	

	SocketSystem sys;
//	m_svr_addr = sys.getHostName();
	cout << "server address is " << m_svr_addr << endl;
	cout << "server port is " << m_svr_port << endl;

	assert(soc.connect(m_svr_addr, m_svr_port));
	Message req;
	req.setCmd("request");
	req.setSenderAddress(m_addr);
	req.setSenderPort(m_port);
	req.setReceiverAddress(m_svr_addr);
	req.setReiverPort(m_svr_port);
	string ss = req.serialize();
	cout << ss << endl;
	
	soc.send(ss);
	cout << "send request ok" << endl;

	ss = soc.recv();

	cout << ss << endl;

	return true;
	*/
}

bool Client::send(const std::string& buf, bool isEnd)
{
	Message req;
	char cmd[64];
	if (isEnd)
	{
		sprintf_s(cmd, "send %s end", m_file_name.c_str());
	}
	else
	{
		sprintf_s(cmd, "send %s", m_file_name.c_str());
	}
	
	req.setCmd(string(cmd));
	req.setReceiverAddress(m_svr_addr);
	req.setSenderAddress(m_addr);
	req.setContentLen((int)buf.size());
	req.setContent(buf);
	string xml_str = req.serialize();
	cout << xml_str << endl;

	cout << __FILE__ << ", line: " << __LINE__ << endl;

	zmq_send(m_requester, xml_str.c_str(), xml_str.size(), 0);
//	zmq_send(m_requester, "hello", 5, 0);

	cout << __FILE__ << ", line: " << __LINE__ << endl;


	char rec_buf[1024];
	memset(rec_buf, 0, sizeof(rec_buf));
	zmq_recv(m_requester, rec_buf, 1024, 0);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	Message res;
	res.parse(string(rec_buf));
	cout << __FILE__ << ", line: " << __LINE__ << endl;
	cout << res.getCmd() << endl;
	assert(res.getCmd() == "OK");

	if (isEnd)
	{
		zmq_close(m_requester);
		m_requester = NULL;
		zmq_ctx_destroy(m_context);
		m_context = NULL;
	}

	return true;
}

int main(int argc, char* argv[])
{
	string file_name = argv[1];
	Client cli;
	string ip = "127.0.0.1";
	int server_port = 5555;

	if (!cli.init(file_name, ip, server_port))
	{
		cout << "connect to server error" << endl;
		return -1;
	}
	cli.start();
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	return 0;
}