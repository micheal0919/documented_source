#ifndef __WIN_UTILITY_H__
#define __WIN_UTILITY_H__

#include <string>
#include <vector>
#include <winsock.h>

class WinUtility
{
public:
	WinUtility();
	~WinUtility();

	static std::vector<std::string> getLocalIP();
	static bool createDirectoryFromFilePath(const std::string& path);
};

#endif // __WIN_UTILITY_H__
