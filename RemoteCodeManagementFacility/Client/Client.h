#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <string>
#include <vector>
#include <chrono>

typedef struct SFileInfo
{
	std::string path;
	std::string ip;
}FileInfo;

class Client
{
public:
	Client();
	~Client();
	
	bool init(const std::string& addr, int port);
	bool upload(const std::string& path, 
		std::chrono::duration<double>& all_time,
		std::chrono::duration<double>& net_time);
	bool list(const std::string& address,
		const std::string& path,
		std::vector<std::string>& file_name,
		std::vector<std::string>& dir_name,
		std::chrono::duration<double>& all_time,
		std::chrono::duration<double>& net_time);
	bool download(const std::string& address, 
		const std::string& path,
		std::chrono::duration<double>& all_time,
		std::chrono::duration<double>& net_time);
	bool fileSearch(const std::string& file_name, 
		std::vector<FileInfo>& info,
		std::chrono::duration<double>& all_time,
		std::chrono::duration<double>& net_time);
	bool stringSearch(const std::string& str_search, 
		int thread_num, 
		std::vector<FileInfo>& info,
		std::chrono::duration<double>& all_time,
		std::chrono::duration<double>& net_time);
private:
	int send(const char* buf, int len);

private:
	void *m_context;
	void *m_requester;
	std::string m_ip_addr;
};

#endif // __CLIENT_H__
