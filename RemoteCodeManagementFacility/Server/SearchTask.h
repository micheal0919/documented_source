#ifndef __SEARCH_THREAD_H__
#define __SEARCH_THREAD_H__

#include <thread>
#include <string>
#include <vector>

class SearchTask
{
public:
	SearchTask(const std::vector<std::string>& file_name, const std::string& str_search);
	~SearchTask();
	
	void operator()();
	std::vector<std::string> getResult();
	
private:
	void threadProc();

private:
	std::vector<std::string> m_file_name;
	std::string m_str_search;
	std::vector<std::string> m_result;
	bool m_done;
//	std::thread m_thread;
};

#endif // __SEARCH_THREAD_H__
