#include "Server.h"
// #include "SearchTask.h"
#include "zeromq/zmq.h"
#include "zeromq/zmq_utils.h"
#include "../Common/RCMMessage.pb.h"
#include "../Common/FileSystem.h"
#include "../Common/WinUtility.h"
#include <assert.h>
#include <iostream>
#include <thread>

#define BUFLEN 2048

using namespace std;
using namespace FileSystem;

const string con_work_space = "server_work_space";

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

bool Server::init(int port)
{
	//  Socket to talk to clients
	m_context = zmq_ctx_new();
	m_responder = zmq_socket(m_context, ZMQ_REP);
	char buf[64];
	sprintf(buf, "tcp://*:%d", port);
	cout << buf << endl;
	int rc = zmq_bind(m_responder, buf);
	assert(rc == 0);

	if (!Directory::exists(con_work_space))
	{
		Directory::create(con_work_space);
	}

	return true;
}

bool Server::start()
{
	char buf[BUFLEN];
	while (1) {
		memset(buf, 0, BUFLEN);
		int len = zmq_recv(m_responder, buf, BUFLEN, 0);
		if (len == -1)
		{
			cout << "error no is " << zmq_errno() << endl;
			cout << zmq_strerror(zmq_errno()) << endl;

		}
		assert(len != -1);
		cout << __FILE__ << ", line: " << __LINE__ << endl;
		handleInput(buf, len);
		cout << __FILE__ << ", line: " << __LINE__ << endl;
	}
}

bool Server::handleInput(const char* buf, int len)
{
	RCMMessage::Message msg;
	msg.ParseFromString(string(buf, len));

	switch (msg.type())
	{
	case RCMMessage::MSG::Upload_Request:
	{
		bool res = handleUpload(msg);
		assert(res);
		break;
	}
	case RCMMessage::MSG::List_Request:
	{
		bool res = handleList(msg);
		assert(res);
		break;
	}
	case RCMMessage::MSG::Download_Request:
	{
		bool res = handleDownload(msg);
		assert(res);
		break;
	}
	case RCMMessage::MSG::File_Search_Request:
	{
		bool res = handleFileSearch(msg);
		assert(res);
		break;
	}
	case RCMMessage::MSG::String_Search_Request:
	{
		bool res = handleStringSearch(msg);
		assert(res);
		break;
	}

	default:
		;
	}

	cout << __FILE__ << ", line: " << __LINE__ << endl;
	return true;
}

bool Server::handleUpload(const RCMMessage::Message& msg)
{
	int sequence = msg.sequence();
	RCMMessage::Request req_msg = msg.request();
	RCMMessage::UploadRequest up_req_msg = req_msg.upload_req();
	string client_file_name = up_req_msg.filename();
	string ip = up_req_msg.ip();
	string content = up_req_msg.content();

	string server_file_path = con_work_space + '\\' + ip + '\\' + client_file_name;
	WinUtility::createDirectoryFromFilePath(server_file_path);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	static FILE* file = NULL;
	if (up_req_msg.fileset())
	{
		file = ::fopen(server_file_path.c_str(), "w");
		assert(file);
//		m_catalog["ip"].push_back(server_file_path);
		m_catalog.push_back(server_file_path);
		cout << __FILE__ << ", line: " << __LINE__ << endl;
	}
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	int write_len = ::fwrite(content.c_str(), 1, content.size(), file);
	assert(content.size() == write_len);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	if (up_req_msg.fileend())
	{
		::fclose(file);
		file = NULL;
		cout << __FILE__ << ", line: " << __LINE__ << endl;
	}
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::UploadResponse up_res_msg;
	up_res_msg.set_filename(client_file_name);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::Response res_msg;
	res_msg.set_result(true);
	res_msg.set_last_block(1);
	res_msg.set_block_index(1);
	res_msg.mutable_upload_res()->CopyFrom(up_res_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::Message msg_send;
	msg_send.set_type(RCMMessage::MSG::Upload_Response);
	msg_send.set_sequence(sequence);
	msg_send.mutable_response()->CopyFrom(res_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	string msg_str;
	msg_send.SerializeToString(&msg_str);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	int send_len = zmq_send(m_responder, msg_str.c_str(), msg_str.size(), 0);
	assert(send_len == msg_str.size());
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	//delete up_res_msg, up_res_msg = NULL;
	//delete res_msg, res_msg = NULL;
	//delete msg_send, msg_send = NULL;

	cout << __FILE__ << ", line: " << __LINE__ << endl;

	return true;
}

bool Server::handleList(const RCMMessage::Message& msg)
{
	int sequence = msg.sequence();
	RCMMessage::Request req_msg = msg.request();
	RCMMessage::ListRequest ls_req_msg = req_msg.list_req();

	string ip = ls_req_msg.ip();
	string path_cli = ls_req_msg.path();
	string search_path = con_work_space + '\\' + ip + '\\' + path_cli + '\\';
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	//m_catalog.push_back(search_path + "aaaa.txt");
	//m_catalog.push_back(search_path + "bbb\\ccc.txt");

	vector<string> file_name;
	vector<string> dir_name;
	list<string>::iterator it = m_catalog.begin();
	for (; it != m_catalog.end(); ++it)
	{
		size_t pos = it->find(search_path);
		if ( pos != string::npos)
		{
			size_t pos2 = it->find_first_of('\\', pos + search_path.size());
			if (pos2 == string::npos)
			{
				char buf[128];
				memset(buf, 0, 128);
				int len = it->size() - pos - search_path.size();
				it->copy(buf, len, pos + search_path.size());
				cout << __FILE__ << ", line: " << __LINE__ << endl;
				cout << buf << endl;
				file_name.push_back(buf);
			}
			else
			{
				char buf[128];
				memset(buf, 0, 128);
				int len = pos2 - pos - search_path.size();
				it->copy(buf, len, pos + search_path.size());
				cout << __FILE__ << ", line: " << __LINE__ << endl;
				cout << buf << endl;
				dir_name.push_back(buf);
			}
		}
	}
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::ListResponse ls_res_msg;
	ls_res_msg.set_path(path_cli);
	for (size_t i = 0; i < file_name.size(); i++)
	{
		cout << __FILE__ << ", line: " << __LINE__ << endl;
		ls_res_msg.add_filename(file_name[i]);
	}
	for (size_t i = 0; i < dir_name.size(); i++)
	{
		cout << __FILE__ << ", line: " << __LINE__ << endl;
		ls_res_msg.add_dirname(dir_name[i]);
	}
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::Response res_msg;
	res_msg.set_result(true);
	res_msg.set_block_index(0);
	res_msg.set_last_block(0);
	res_msg.mutable_list_res()->CopyFrom(ls_res_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::Message msg_send;
	msg_send.set_type(RCMMessage::MSG::List_Response);
	msg_send.set_sequence(sequence);
	msg_send.mutable_response()->CopyFrom(res_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	string str_msg;
	msg_send.SerializeToString(&str_msg);

	int send_len = zmq_send(m_responder, str_msg.c_str(), str_msg.size(), 0);
	assert(send_len == str_msg.size());
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	return true;
}

bool Server::handleDownload(const RCMMessage::Message& msg)
{
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	int sequence = msg.sequence();
	RCMMessage::Request req_msg = msg.request();
	RCMMessage::DownloadRequest down_req_msg = req_msg.download_req();
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	string path_cli = down_req_msg.path();
	cout << __FILE__ << ", line: " << __LINE__ << endl;
	cout << path_cli << endl;
	string ip = down_req_msg.ip();
	cout << __FILE__ << ", line: " << __LINE__ << endl;
	cout << ip << endl;
	string path_svr = con_work_space + '\\' + ip + '\\' + path_cli;
	cout << __FILE__ << ", line: " << __LINE__ << endl;
	cout << path_svr << endl;

	bool isFound = false;
	//list<string>::iterator it = m_catalog.begin();
	//for (; it != m_catalog.end(); ++it)
	//{
	//	if (*it == path_svr)
	//	{
	//		isFound = true;
	//	}
	//}

	list<string>::iterator it = m_catalog.end();
	it = std::find(m_catalog.begin(), m_catalog.end(), path_svr);
	if (it != m_catalog.end())
	{
		isFound = true;
		cout << __FILE__ << ", line: " << __LINE__ << endl;
	}
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	if (isFound == false)
	{
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		RCMMessage::DownloadResponse down_res_msg;
		down_res_msg.set_path(path_cli);
		down_res_msg.set_fileset(false);
		down_res_msg.set_fileend(false);
		string content = "";
		down_res_msg.set_content(content);
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		RCMMessage::Response res_msg;
		res_msg.set_result(false);
		string error_str = "Cannot find the file the server catalog";
		res_msg.set_error_description(error_str);
		res_msg.set_last_block(0);
		res_msg.set_block_index(0);
		res_msg.mutable_download_res()->CopyFrom(down_res_msg);
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		RCMMessage::Message msg_send;
		msg_send.set_type(RCMMessage::MSG::Download_Response);
		msg_send.set_sequence(sequence);
		msg_send.mutable_response()->CopyFrom(res_msg);
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		string str_msg;
		msg_send.SerializeToString(&str_msg);
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		int send_len = zmq_send(m_responder, str_msg.c_str(), str_msg.size(), 0);
		assert(send_len == str_msg.size());
		cout << __FILE__ << ", line: " << __LINE__ << endl;
	}
	else
	{
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		RCMMessage::DownloadResponse down_res_msg;
		down_res_msg.set_path(path_cli);

		static FILE* file = NULL;

		if (file == NULL)
		{
			file = ::fopen(path_svr.c_str(), "r");
			assert(file);
			down_res_msg.set_fileset(true);
			cout << __FILE__ << ", line: " << __LINE__ << endl;
		}
		else
		{
			down_res_msg.set_fileset(false);
			cout << __FILE__ << ", line: " << __LINE__ << endl;
		}

		char buf[1024];
		memset(buf, 0, 1024);
		int len = ::fread(buf, 1, 1024, file);
		assert(len != -1);
		if (len < 1024)
		{
			down_res_msg.set_fileend(true);
			::fclose(file);
			file = NULL;
			cout << __FILE__ << ", line: " << __LINE__ << endl;
		}
		else if (len == 1024)
		{
			down_res_msg.set_fileend(false);
			cout << __FILE__ << ", line: " << __LINE__ << endl;
		}

		down_res_msg.set_content(buf, len);
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		RCMMessage::Response res_msg;
		res_msg.set_result(true);
		res_msg.set_last_block(0);
		res_msg.set_block_index(0);
		res_msg.mutable_download_res()->CopyFrom(down_res_msg);
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		RCMMessage::Message msg_send;
		msg_send.set_type(RCMMessage::MSG::Download_Response);
		msg_send.set_sequence(sequence);
		msg_send.mutable_response()->CopyFrom(res_msg);
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		string str_msg;
		msg_send.SerializeToString(&str_msg);
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		int send_len = zmq_send(m_responder, str_msg.c_str(), str_msg.size(), 0);
		assert(send_len == str_msg.size());
		cout << __FILE__ << ", line: " << __LINE__ << endl;

	}
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	return true;
}

bool Server::handleFileSearch(const RCMMessage::Message& msg)
{
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	int sequence = msg.sequence();
	RCMMessage::Request req_msg = msg.request();
	RCMMessage::FileSearchRequest file_req_msg = req_msg.file_search_req();
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	string file_name = file_req_msg.filename();

//	m_catalog.push_back(con_work_space + "\\10.6.5.78\\aaaa\\" + file_name);

	RCMMessage::FileSearchResponse file_res_msg;
	file_res_msg.set_filename(file_name);
	list<string>::iterator it = m_catalog.begin();
	//bool isFound = false;
	for (; it != m_catalog.end(); ++it)
	{
		if (it->find(file_name) != string::npos)
		{
			//isFound = true;
			cout << __FILE__ << ", line: " << __LINE__ << endl;

			RCMMessage::FileSearchResponse_fileInfo* info = file_res_msg.add_info();

			size_t pos = it->find_first_of('\\', con_work_space.size() + 1);
			char buf[64];
			memset(buf, 0, 64);
			it->copy(buf, pos - con_work_space.size() -1, con_work_space.size() + 1);
			cout << buf << endl;
			info->set_ip(buf);

			memset(buf, 0, 64);
			it->copy(buf, it->size() - pos - 1, pos + 1);
			cout << buf << endl;
			info->set_path(buf);
			cout << __FILE__ << ", line: " << __LINE__ << endl;

		}
	}
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::Response res_msg;
	res_msg.set_result(true);
	res_msg.set_last_block(0);
	res_msg.set_block_index(0);
	res_msg.mutable_file_search_res()->CopyFrom(file_res_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::Message msg_send;
	msg_send.set_type(RCMMessage::MSG::File_Search_Response);
	msg_send.set_sequence(sequence);
	msg_send.mutable_response()->CopyFrom(res_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	string str_msg;
	msg_send.SerializeToString(&str_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	int send_len = zmq_send(m_responder, str_msg.c_str(), str_msg.size(), 0);
	assert(send_len == str_msg.size());
	cout << __FILE__ << ", line: " << __LINE__ << endl;


	return true;
}

bool Server::handleStringSearch(const RCMMessage::Message& msg)
{
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	int sequence = msg.sequence();
	RCMMessage::Request req_msg = msg.request();
	RCMMessage::StringSearchRequest str_req_msg = req_msg.string_search_req();
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	string str_search = str_req_msg.search();
	int thread_num = str_req_msg.threadnum();

	//	m_catalog.push_back(con_work_space + "\\10.6.5.78\\aaaa\\" + file_name);

	RCMMessage::StringSearchResponse str_res_msg;
	str_res_msg.set_search(str_search);

	
	int left = m_catalog.size() % thread_num;
	vector<int> nums;
	int sum = 0;
	for (int i = 0; i < thread_num; i++)
	{
		if (i < left)
		{
			nums.push_back(m_catalog.size() / thread_num + 1);
		}
		nums.push_back(m_catalog.size() / thread_num);
		sum += nums[i];
	}
	assert(sum == (int)m_catalog.size());

	
	
//	thread* p_thread = new thread[thread_num];
//	assert(p_thread);

	//thread* p_ts = new thread[thread_num];
	//assert(p_ts);
	//vector<SearchTask> vec_task;
	//list<string>::iterator it = m_catalog.begin();

	//vector< vector<string> > vec_files;
	//for (int i = 0; i < thread_num; i++)
	//{
	//	for (int j = 0; j < nums[i]; j++)
	//	{
	//		vector<string> files;
	//		files.push_back(*it);
	//		vec_files.push_back(files);
	//		++it;
	//	}
	//	SearchTask task(vec_files[i], str_search);
	//	vec_task.push_back(task);
	//	*(p_ts + i) = thread(task);
	//}

	//for (int i = 0; i < thread_num; i++)
	//{
	//	(p_ts + i)->join();
	//}
	//

	//for (int i = 0; i < thread_num; i++)
	//{
	//	vector<string> res = vec_task[i].getResult();
	//	for (size_t j = 0; j < res.size(); j++)
	//	{
	//		cout << __FILE__ << ", line: " << __LINE__ << endl;

	//		RCMMessage::StringSearchResponse_fileInfo* info = str_res_msg.add_info();

	//		size_t pos = it->find_first_of('\\', con_work_space.size() + 1);
	//		char buf[64];
	//		memset(buf, 0, 64);
	//		it->copy(buf, pos - con_work_space.size() - 1, con_work_space.size() + 1);
	//		cout << buf << endl;
	//		info->set_ip(buf);

	//		memset(buf, 0, 64);
	//		it->copy(buf, it->size() - pos - 1, pos + 1);
	//		cout << buf << endl;
	//		info->set_path(buf);
	//		cout << __FILE__ << ", line: " << __LINE__ << endl;
	//	}
	//}


	list<string>::iterator it = m_catalog.begin();
	for (; it != m_catalog.end(); ++it)
	{
		bool isFound = false;

		FILE* file = ::fopen(it->c_str(), "r");
		assert(file);
		char buf[BUFLEN];

		string scope;

		while (true)
		{
			memset(buf, 0, BUFLEN);
			int len = ::fread(buf, 1, BUFLEN, file);
			assert(len != -1);

			scope = scope + string(buf);

			if (::strstr(scope.c_str(), str_search.c_str()) != NULL)
			{
				cout << __FILE__ << ", line: " << __LINE__ << endl;
				isFound = true;
				break;
			}
			if (len < BUFLEN)
			{
				cout << __FILE__ << ", line: " << __LINE__ << endl;
				break;
			}
			scope = string(buf);
		}
		cout << __FILE__ << ", line: " << __LINE__ << endl;

		::fclose(file);
		file = NULL;

		if (isFound)
		{
			cout << __FILE__ << ", line: " << __LINE__ << endl;

			RCMMessage::StringSearchResponse_fileInfo* info = str_res_msg.add_info();

			size_t pos = it->find_first_of('\\', con_work_space.size() + 1);
			char buf[64];
			memset(buf, 0, 64);
			it->copy(buf, pos - con_work_space.size() - 1, con_work_space.size() + 1);
			cout << buf << endl;
			info->set_ip(buf);

			memset(buf, 0, 64);
			it->copy(buf, it->size() - pos - 1, pos + 1);
			cout << buf << endl;
			info->set_path(buf);
			cout << __FILE__ << ", line: " << __LINE__ << endl;

		}
	}
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::Response res_msg;
	res_msg.set_result(true);
	res_msg.set_last_block(0);
	res_msg.set_block_index(0);
	res_msg.mutable_string_search_res()->CopyFrom(str_res_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	RCMMessage::Message msg_send;
	msg_send.set_type(RCMMessage::MSG::String_Search_Response);
	msg_send.set_sequence(sequence);
	msg_send.mutable_response()->CopyFrom(res_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	string str_msg;
	msg_send.SerializeToString(&str_msg);
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	int send_len = zmq_send(m_responder, str_msg.c_str(), str_msg.size(), 0);
	assert(send_len == str_msg.size());
	cout << __FILE__ << ", line: " << __LINE__ << endl;

//	delete []p_ts;
	cout << __FILE__ << ", line: " << __LINE__ << endl;

	return true;
}

int main(int argc, char* argv[])
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	int port = 0;
	sscanf(argv[1], "%d", &port);

	Server svr;
	assert(svr.init(port));
	svr.start();

	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}