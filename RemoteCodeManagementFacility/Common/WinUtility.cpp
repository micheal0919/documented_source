#include "WinUtility.h"
#include "FileSystem.h"
#include <iostream>

using namespace std;
using namespace FileSystem;

WinUtility::WinUtility()
{
}


WinUtility::~WinUtility()
{
}

std::vector<std::string> WinUtility::getLocalIP()
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
}

bool WinUtility::createDirectoryFromFilePath(const std::string& path)
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
