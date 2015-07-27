#include "Client.h"
#include "zeromq/zmq.h"
#include "zeromq/zmq_utils.h"
#include "../Common/RCMMessage.pb.h"
#include "../Common/WinUtility.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>

#define BUFLEN 1024

using namespace std;
using namespace std::chrono;

static int st_sequence = 0;

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


bool Client::init(const std::string& addr, int port)
{
	vector<string> ip = WinUtility::getLocalIP();
	assert(ip.size() > 0);
	m_ip_addr = ip[0];

	m_context = zmq_ctx_new();
	m_requester = zmq_socket(m_context, ZMQ_REQ);

	char buf[64];
	sprintf(buf, "tcp://%s:%d", addr.c_str(), port);
	std::cout << buf << endl;
	int rc = zmq_connect(m_requester, buf);
	if (rc != 0)
	{
		std::cout << zmq_strerror(zmq_errno()) << endl;
	}
	assert(rc == 0);
	return true;
}

bool Client::upload(const std::string& file_path,
	std::chrono::duration<double>& all_time,
	std::chrono::duration<double>& net_time)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	cout << file_path << endl;

	FILE* file = ::fopen(file_path.c_str(), "r");
	assert(file);

	char buf[BUFLEN];
	int count = 0;
	while (true)
	{
		count++;
		st_sequence++;
		memset(buf, 0, BUFLEN);
		int len = fread(buf, 1, BUFLEN, file);

		RCMMessage::UploadRequest up_req_msg;
		up_req_msg.set_filename(file_path);
		up_req_msg.set_ip(m_ip_addr);
		if (count == 1)
		{
			up_req_msg.set_fileset(true);
		}
		else
		{
			up_req_msg.set_fileset(false);
		}

		if (len < BUFLEN)
		{
			up_req_msg.set_fileend(true);
		}
		else
		{
			up_req_msg.set_fileend(false);
		}

		//if (count == 1 && len < BUFLEN)
		//{
		//	up_req_msg.set_status(RCMMessage::UploadRequest::ContentStatus::UploadRequest_ContentStatus_ending);
		//}
		//else if (count == 1 && len == BUFLEN)
		//{
		//	up_req_msg.set_status(RCMMessage::UploadRequest::ContentStatus::UploadRequest_ContentStatus_start);
		//}
		//else if (count > 1 && len < BUFLEN)
		//{
		//	up_req_msg.set_status(RCMMessage::UploadRequest::ContentStatus::UploadRequest_ContentStatus_ending);
		//}
		//else if (count > 1 && len == BUFLEN)
		//{
		//	up_req_msg.set_status(RCMMessage::UploadRequest::ContentStatus::UploadRequest_ContentStatus_middle);
		//}

		up_req_msg.set_content(buf, len);

		RCMMessage::Request req_msg;
		req_msg.mutable_upload_req()->CopyFrom(up_req_msg);

		RCMMessage::Message msg;
		msg.set_type(RCMMessage::MSG::Upload_Request);
		msg.set_sequence(st_sequence);
		
		msg.mutable_request()->CopyFrom(req_msg);

		string msg_str;
		msg.SerializeToString(&msg_str);

		high_resolution_clock::time_point t3 = high_resolution_clock::now();

		int send_len = zmq_send(m_requester, msg_str.c_str(), msg_str.size(), 0);
		assert(send_len == msg_str.size());
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		//delete msg, msg = NULL;
		//delete req_msg, req_msg = NULL;
		//delete up_req_msg, up_req_msg = NULL;

		cout << __FILE__ << ", line: " << __LINE__ << endl;

		char rcv_buf[BUFLEN];
		memset(rcv_buf, 0, BUFLEN);
		int rcv_len = zmq_recv(m_requester, rcv_buf, BUFLEN, 0);

		high_resolution_clock::time_point t4 = high_resolution_clock::now();

		net_time += duration_cast<duration<double>>(t4 - t3);

		cout << __FILE__ << ", line: " << __LINE__ << endl;
		
		RCMMessage::Message msg_rcv;


		msg_rcv.ParseFromString(string(rcv_buf, rcv_len));
		assert(msg_rcv.type() == RCMMessage::MSG::Upload_Response);
		assert(msg_rcv.sequence() == st_sequence);
		
		RCMMessage::Response res_msg = msg_rcv.response();
		if (res_msg.result() != true)
		{
			std::cout << res_msg.error_description() << endl;
			assert(0);
			return false;
		}

		RCMMessage::UploadResponse up_res_msg = res_msg.upload_res();
		assert(up_res_msg.filename() == file_path);


		if (len < BUFLEN)
		{
			std::cout << __FILE__ << ", line: " << __LINE__ <<std:: endl;
			break;
		}

		std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;
	}
	::fclose(file);
	std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;

	zmq_close(m_requester);
	m_requester = NULL;
	zmq_ctx_destroy(m_context);
	m_context = NULL;
	std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	all_time = duration_cast<duration<double>>(t2 - t1);

	return true;
}

bool Client::list(const std::string& address,
	const std::string& path,
	std::vector<std::string>& file_name,
	std::vector<std::string>& dir_name,
	std::chrono::duration<double>& all_time,
	std::chrono::duration<double>& net_time)
{
	RCMMessage::ListRequest ls_req_msg;
	ls_req_msg.set_ip(address);
	ls_req_msg.set_path(path);

	RCMMessage::Request req_msg;
	req_msg.mutable_list_req()->CopyFrom(ls_req_msg);

	RCMMessage::Message msg;
	msg.set_type(RCMMessage::MSG::List_Request);
	st_sequence++;
	msg.set_sequence(st_sequence);
	msg.mutable_request()->CopyFrom(req_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	string str_msg;
	msg.SerializeToString(&str_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	int send_len = zmq_send(m_requester, str_msg.c_str(), str_msg.size(), 0);
	assert(send_len == str_msg.size());
	cout << __FILE__ << ", line: " << __LINE__ << endl;



	char rcv_buf[BUFLEN];
	memset(rcv_buf, 0, BUFLEN);
	int rcv_len = zmq_recv(m_requester, rcv_buf, BUFLEN, 0);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	zmq_close(m_requester);
	m_requester = NULL;
	zmq_ctx_destroy(m_context);
	m_context = NULL;
	std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;

	RCMMessage::Message msg_rcv;
	msg_rcv.ParseFromString(string(rcv_buf, rcv_len));
	assert(msg_rcv.type() == RCMMessage::MSG::List_Response);
	assert(msg_rcv.sequence() == st_sequence);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::Response res_msg = msg_rcv.response();
	if (res_msg.result() != true)
	{
		std::cout << res_msg.error_description() << endl;
		assert(0);
		return false;
	}
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::ListResponse ls_res_msg = res_msg.list_res();
	assert(ls_res_msg.path()== path);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	file_name.clear();
	int file_num = ls_res_msg.filename_size();
	for (int i = 0; i < file_num; i++)
	{
		file_name.push_back(ls_res_msg.filename(i));
	}
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	dir_name.clear();
	int dir_num = ls_res_msg.dirname_size();
	for (int i = 0; i < dir_num; i++)
	{
		dir_name.push_back(ls_res_msg.dirname(i));
	}
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	return true;
}

bool Client::download(const std::string& address, const std::string& path,
	std::chrono::duration<double>& all_time,
	std::chrono::duration<double>& net_time)
{
	FILE* file = NULL;
	while (true)
	{
		RCMMessage::DownloadRequest down_req_msg;
		down_req_msg.set_ip(address);
		down_req_msg.set_path(path);

		RCMMessage::Request req_msg;
		req_msg.mutable_download_req()->CopyFrom(down_req_msg);

		RCMMessage::Message msg;
		msg.set_type(RCMMessage::MSG::Download_Request);
		st_sequence++;
		msg.set_sequence(st_sequence);
		msg.mutable_request()->CopyFrom(req_msg);

		string str_msg;
		msg.SerializeToString(&str_msg);
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		int send_len = zmq_send(m_requester, str_msg.c_str(), str_msg.size(), 0);
		assert(send_len == str_msg.size());
		cout << __FILE__ << ", line: " << __LINE__ << endl;


		char rcv_buf[2048];
		memset(rcv_buf, 0, 2048);
		int rcv_len = zmq_recv(m_requester, rcv_buf, 2048, 0);
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		RCMMessage::Message msg_rcv;
		msg_rcv.ParseFromString(string(rcv_buf, rcv_len));
		assert(msg_rcv.type() == RCMMessage::MSG::Download_Response);
		assert(msg_rcv.sequence() == st_sequence);
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		RCMMessage::Response res_msg = msg_rcv.response();
		if (res_msg.result() != true)
		{
			std::cout << res_msg.error_description() << endl;
			assert(0);
			return false;
		}
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		RCMMessage::DownloadResponse down_res_msg;
		down_res_msg = res_msg.download_res();
		string path_cli = down_res_msg.path();
		cout << __FILE__ << ", line: " << __LINE__ << endl;
		cout << path << endl;
		cout << path_cli << endl;
		assert(path == path_cli);
		path_cli = "client_download\\" + path;
		WinUtility::createDirectoryFromFilePath(path_cli);

		string content = down_res_msg.content();

		if (down_res_msg.fileset())
		{
			assert(file == NULL);
			file = ::fopen(path_cli.c_str(), "w");
			assert(file);
		}

		int write_len = ::fwrite(content.c_str(), 1, content.size(), file);
		assert(write_len == content.size());

		if (down_res_msg.fileend())
		{
			assert(file);
			::fclose(file);
			file = NULL;
			break;
		}

	}

	zmq_close(m_requester);
	m_requester = NULL;
	zmq_ctx_destroy(m_context);
	m_context = NULL;
	std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;

	return true;

}

bool Client::fileSearch(const std::string& file_name, 
	std::vector<FileInfo>& info,
	std::chrono::duration<double>& all_time,
	std::chrono::duration<double>& net_time)
{
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::FileSearchRequest file_req_msg;
	file_req_msg.set_filename(file_name);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::Request req_msg;
	req_msg.mutable_file_search_req()->CopyFrom(file_req_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::Message msg;
	msg.set_type(RCMMessage::MSG::File_Search_Request);
	st_sequence++;
	msg.set_sequence(st_sequence);
	msg.mutable_request()->CopyFrom(req_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	string str_msg;
	msg.SerializeToString(&str_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	int send_len = zmq_send(m_requester, str_msg.c_str(), str_msg.size(), 0);
	assert(send_len == str_msg.size());
	cout << __FILE__ << ", line: " << __LINE__ << endl;



	char rcv_buf[BUFLEN];
	memset(rcv_buf, 0, BUFLEN);
	int rcv_len = zmq_recv(m_requester, rcv_buf, BUFLEN, 0);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	zmq_close(m_requester);
	m_requester = NULL;
	zmq_ctx_destroy(m_context);
	m_context = NULL;
	std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;

	RCMMessage::Message msg_rcv;
	msg_rcv.ParseFromString(string(rcv_buf, rcv_len));
	assert(msg_rcv.type() == RCMMessage::MSG::File_Search_Response);
	assert(msg_rcv.sequence() == st_sequence);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::Response res_msg = msg_rcv.response();
	if (res_msg.result() != true)
	{
		std::cout << res_msg.error_description() << endl;
		assert(0);
		return false;
	}
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::FileSearchResponse file_res_msg = res_msg.file_search_res();
	assert(file_name == file_res_msg.filename());
	int file_num = file_res_msg.info_size();
	info.clear();
	for (int i = 0; i < file_num; i++)
	{
		RCMMessage::FileSearchResponse::fileInfo info_res = file_res_msg.info(i);
		FileInfo info_one;
		info_one.path = info_res.path();
		info_one.ip = info_res.ip();
		info.push_back(info_one);
	}
	cout << __FILE__ << ", line: " << __LINE__ << endl;
	
	return true;
}

bool Client::stringSearch(const std::string& str_search, int thread_num, std::vector<FileInfo>& info,
	std::chrono::duration<double>& all_time,
	std::chrono::duration<double>& net_time)
{

	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::StringSearchRequest str_req_msg;
	str_req_msg.set_search(str_search);
	str_req_msg.set_threadnum(thread_num);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::Request req_msg;
	req_msg.mutable_string_search_req()->CopyFrom(str_req_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::Message msg;
	msg.set_type(RCMMessage::MSG::String_Search_Request);
	st_sequence++;
	msg.set_sequence(st_sequence);
	msg.mutable_request()->CopyFrom(req_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	string str_msg;
	msg.SerializeToString(&str_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	int send_len = zmq_send(m_requester, str_msg.c_str(), str_msg.size(), 0);
	assert(send_len == str_msg.size());
	cout << __FILE__ << ", line: " << __LINE__ << endl;



	char rcv_buf[BUFLEN];
	memset(rcv_buf, 0, BUFLEN);
	int rcv_len = zmq_recv(m_requester, rcv_buf, BUFLEN, 0);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	zmq_close(m_requester);
	m_requester = NULL;
	zmq_ctx_destroy(m_context);
	m_context = NULL;
	std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;

	RCMMessage::Message msg_rcv;
	msg_rcv.ParseFromString(string(rcv_buf, rcv_len));
	assert(msg_rcv.type() == RCMMessage::MSG::String_Search_Response);
	assert(msg_rcv.sequence() == st_sequence);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::Response res_msg = msg_rcv.response();
	if (res_msg.result() != true)
	{
		std::cout << res_msg.error_description() << endl;
		assert(0);
		return false;
	}
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::StringSearchResponse str_res_msg = res_msg.string_search_res();
	assert(str_search == str_res_msg.search());
	int file_num = str_res_msg.info_size();
	info.clear();
	for (int i = 0; i < file_num; i++)
	{
		RCMMessage::StringSearchResponse_fileInfo info_res = str_res_msg.info(i);
		FileInfo info_one;
		info_one.path = info_res.path();
		info_one.ip = info_res.ip();
		info.push_back(info_one);
	}
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	return true;
}

int Client::send(const char* buf, int len)
{
	zmq_send(m_requester,buf, len, 0);
	Sleep(10);
	char rec_buf[20];
	zmq_recv(m_requester, rec_buf, 20, 0);
	cout << rec_buf << endl;
	return 0;
}

int main(int argc, char* argv[])
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	string addr = argv[1];
	int port = 0;
	sscanf(argv[2], "%d", &port);
	string path;
	Client cli;
	vector<string> ip = WinUtility::getLocalIP();
	std::chrono::duration<double> all_time;
	std::chrono::duration<double> net_time;

	assert(cli.init(addr, port));
	if (::strcmp(argv[3], "upload") == 0)
	{
		path = argv[4];
		bool res = cli.upload(path, all_time, net_time);
		assert(res);
	}
	else if (::strcmp(argv[3], "list") == 0)
	{
		path = argv[4];
		
		vector<string> file_name;
		vector<string> dir_name;

		bool res = cli.list(ip[0], path, file_name, dir_name, all_time, net_time);
		assert(res);
		for (size_t i = 0; i < file_name.size(); i++)
		{
			cout << __FILE__ << ", line: " << __LINE__ << endl;
			cout << "file " << i << " is " << file_name[i] << endl;
		}
		for (size_t i = 0; i < dir_name.size(); i++)
		{
			cout << __FILE__ << ", line: " << __LINE__ << endl;
			cout << "dir " << i << " is " << dir_name[i] << endl;
		}
	}
	else if (::strcmp(argv[3], "download") == 0)
	{
		path = argv[4];
		bool res = cli.download(ip[0], path, all_time, net_time);
		assert(res);
	}
	else if (::strcmp(argv[3], "file_search") == 0)
	{
		cout << __FILE__ << ", line: " << __LINE__ << endl;
		string file_name = argv[4];
		vector<FileInfo> info;
		bool res = cli.fileSearch(file_name, info, all_time, net_time);
		for (size_t i = 0; i < info.size(); i++)
		{
			cout << "file " << i << " is " << info[i].path << endl;
			cout << "file " << i << " ip address is " << info[i].ip << endl;
		}
		assert(res);
	}
	else if (::strcmp(argv[3], "string_search") == 0)
	{
		cout << __FILE__ << ", line: " << __LINE__ << endl;
		string str_search = argv[4];
		int thread_num = 0;
		sscanf(argv[5], "%d", &thread_num);
		vector<FileInfo> info;
		bool res = cli.stringSearch(str_search, thread_num, info, all_time, net_time);
		for (size_t i = 0; i < info.size(); i++)
		{
			cout << "file contents the string " << i << " is " << info[i].path << endl;
			cout << "file contents the string " << i << " ip address is " << info[i].ip << endl;
		}
		assert(res);
	}

	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}