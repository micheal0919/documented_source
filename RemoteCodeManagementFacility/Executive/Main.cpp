
#include <shlobj.h>
#include <WinUser.h>
#include <windows.h>
#include <iostream>

using namespace std;

class Executive
{
public:
	Executive();
};

Executive::Executive()
{
	//open .exe files
	ShellExecute(0, L"open", L"server.exe", L"38000", NULL, SW_SHOWNORMAL);
	//	ShellExecute(0, L"open", L"..\\Debug\\Server.exe", L"9080", L"..\\ServerFolder", SW_SHOWNORMAL);
	ShellExecute(0, L"open", L"client.exe", L"127.0.0.1 38000 upload client_work_space\\folly-master\\folly\\AtomicBitSet.h", NULL, SW_SHOWNORMAL);
	ShellExecute(0, L"open", L"client.exe", L"127.0.0.1 38000 upload client_work_space\\folly-master\\folly\\Arena.h", NULL, SW_SHOWNORMAL);
	ShellExecute(0, L"open", L"client.exe", L"127.0.0.1 38000 list client_work_space\\folly-master\\folly", NULL, SW_SHOWNORMAL);
	ShellExecute(0, L"open", L"client.exe", L"127.0.0.1 38000 download client_work_space\\folly-master\\folly\\Arena.h", NULL, SW_SHOWNORMAL);
	
	ShellExecute(0, L"open", L"client.exe", L"127.0.0.1 38000 file_search Arena.h", NULL, SW_SHOWNORMAL);

	ShellExecute(0, L"open", L"client.exe", L"127.0.0.1 38000 string_search public 2", NULL, SW_SHOWNORMAL);


	//	ShellExecute(0, L"open", L"..\\Debug\\Client.exe", L"localhost 9002 upload_text_file localhost 9080 testAtClient.exe download_file localhost 9080 testAtServer.exe binary", L"..\\ClientFolder", SW_SHOWNORMAL);
}

void main()
{
	cout << "Project #4 Demonstration:" << endl;
	cout << "\nPlease look at details of activities on the two consoles.";
	cout << "\n Client at localhost" << endl;
	cout << "\n Server at localhost 38000:";
	cout << "\n";
	cout << "\nClient2 will upload two text files to Server1";
	cout << "\n";
	cout << "\nAfter Client uploaded two text files to Server, Client1 will connect to Server, Server will store the file in it work path.";
	cout << "\n";

	cout << "\n client will explore a directory, show the file in the directory\n";

	cout << "\n client will download a file form the server\n";

	cout << "\n client will find the the file path with the file name\n";

	cout << "\n client will search all file with a string\n";


	Executive ex;
	system("pause");
}
