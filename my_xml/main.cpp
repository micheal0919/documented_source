#include "xmltest.h"
#include <iostream>
#include <string>

using namespace std;


int main(int argc, char* argv[])
{
	if (3 != argc)
	{
		cout << "parameter error, line: " << __LINE__ << endl;
		return -1;
	}
	string cmd = argv[1];
	string fileName = argv[2];
	xmlTest test;
	if (cmd == "true")
	{
		test.ReadXml(fileName);
		return 0;
	}
	else if (cmd == "false")
	{
		test.CreateXml(fileName);
		test.ReadXml(fileName);
		test.SelectSingleNode(fileName, "Person", "ID", "0");
		test.SelectNode(fileName, "Person");
		test.InsertNode(fileName, "Person", "ID", "0", "address", "New York City");
		test.RemoveNode(fileName, "Person", "ID", "5");
		test.getNodeAttributes(fileName, "Person", "ID", "6");
		test.addAttribute(fileName, "Person", "ID", "6", "address", "New York City");
		test.removeAttribute(fileName, "Person", "ID", "6", "name");
		return 0;
	}
	else
	{
		cout << "parameter error, line: " << __LINE__ << endl;
		return -1;
	}

	return 0;
}

