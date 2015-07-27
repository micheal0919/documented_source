
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
	ShellExecute(0, L"open", L"server.exe", NULL, NULL, SW_SHOWNORMAL);
//	ShellExecute(0, L"open", L"..\\Debug\\Server.exe", L"9080", L"..\\ServerFolder", SW_SHOWNORMAL);
	ShellExecute(0, L"open", L"client.exe", L"../common/Message.cpp", NULL, SW_SHOWNORMAL);
//	ShellExecute(0, L"open", L"..\\Debug\\Client.exe", L"localhost 9002 upload_text_file localhost 9080 testAtClient.exe download_file localhost 9080 testAtServer.exe binary", L"..\\ClientFolder", SW_SHOWNORMAL);
}

void main()
{
	cout << "Project #3 Demonstration:" << endl;
	cout << "\nPlease look at details of activities on the two consoles.";
	cout << "\nThere are one client and two server:";
	cout << "\n Client1 at localhost" << endl;;
	cout << "\n Server1 at localhost 5555:";
	cout << "\n";
	cout << "\nClient1 will upload a text file to Server1";
	cout << "\n";
	cout << "\nAfter Client uploaded a text file to Server, Client1 will connect to Server, Server will store the file in it work path.";

	Executive ex;
	system("pause");
}