
#include <iostream>
#include <string>
#include <vector>
#include <winsock.h>

#include "../Common/WinUtility.h"
#include "../Common/FileSystem.h"
#include "../Common/RCMMessage.pb.h"

using namespace std;
using namespace FileSystem;
/*
vector<string> getLocalIP()
{
	vector<string> res;
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		return res;
	}

	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		cerr << "Error " << WSAGetLastError() <<
			" when getting local host name." << endl;
		return res;
	}
	cout << "Host name is " << ac << "." << endl;

	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) {
		cerr << "Yow! Bad host lookup." << endl;
		return res;
	}

	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		cout << "Address " << i << ": " << inet_ntoa(addr) << endl;
		res.push_back(string(inet_ntoa(addr)));
	}

	WSACleanup();

	return res;
}*/

bool createDirectoryFromFilePath(const std::string& path)
{
	size_t pos = -1;
	while (true)
	{
		pos = path.find_first_of('\\', pos + 1);
		if (pos == string::npos)
		{
			break;
		}
		string path(path, 0, pos);
		cout << path << endl;
		if (!Directory::exists(path))
		{
			Directory::create(path);
		}

	}

	return true;
}

bool testMessage()
{
	RCMMessage::UploadRequest up_req_msg;
	up_req_msg.set_filename("a.b");
	up_req_msg.set_ip("bbbb");
	up_req_msg.set_fileset(true);
	up_req_msg.set_fileend(true);
	up_req_msg.set_content("aaaaa");
	std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;

	RCMMessage::Request req_msg;
	req_msg.mutable_upload_req()->CopyFrom(up_req_msg);
	std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;

	RCMMessage::Message msg;
	msg.set_type(RCMMessage::MSG::Upload_Request);
	msg.set_sequence(1);
	msg.mutable_request()->CopyFrom(req_msg);
	std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;

	string str_msg;
	msg.SerializeToString(&str_msg);
	std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;

	return true;
}


bool testMessage_new()
{
	RCMMessage::UploadRequest* up_req_msg = new RCMMessage::UploadRequest();
	up_req_msg->set_filename("a.b");
	up_req_msg->set_ip("bbbb");
	up_req_msg->set_fileset(true);
	up_req_msg->set_fileend(true);
	up_req_msg->set_content("aaaaa");
	std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;

	RCMMessage::Request* req_msg = new RCMMessage::Request();
	req_msg->set_allocated_upload_req(up_req_msg);
	std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;


	RCMMessage::Message* msg = new RCMMessage::Message();
	msg->set_type(RCMMessage::MSG::Upload_Request);
	msg->set_sequence(1);
	msg->set_allocated_request(req_msg);
	std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;

	string str_msg;
	msg->SerializeToString(&str_msg);
	std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;

	delete msg, msg = NULL;
	std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;
	//delete req_msg, req_msg = NULL;
	//std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;
	//delete up_req_msg, up_req_msg = NULL;
	//std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;

	return true;

}
int main(int argc, char *argv[])
{
	
	string path = "client\\bull\\shit\\asshole\\fuck.txt";
	
	
//	createDirectoryFromFilePath(path);



	/*vector<string> res = WinUtility::getLocalIP();
	cout << "res.size() = " << res.size() << endl;
	*/

	testMessage();

//	testMessage_new();

	return 0;
}
