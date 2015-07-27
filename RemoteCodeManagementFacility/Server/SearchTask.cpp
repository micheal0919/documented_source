#include "SearchTask.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <iostream>

#define BUFLEN 1024

using namespace std;

SearchTask::SearchTask(const std::vector<std::string>& file_name, const std::string& str_search)
{
	m_file_name = file_name;
	m_str_search = str_search;
	threadProc();
}


SearchTask::~SearchTask()
{
}

void SearchTask::operator()()
{
	threadProc();
}

std::vector<std::string> SearchTask::getResult()
{
	return m_result;
}

void SearchTask::threadProc()
{
	vector<string>::iterator it = m_file_name.begin();

	for (; it != m_file_name.end(); ++it)
	{
		bool isFound = false;
		cout << __FILE__ << ", line: " << __LINE__ << endl;
		cout << *it << endl;

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

			if (::strstr(scope.c_str(), m_str_search.c_str()) != NULL)
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
			m_result.push_back(*it);
		}
	}
}