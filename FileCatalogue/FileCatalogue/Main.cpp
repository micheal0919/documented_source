#include "FileCatalogue.h"
#include "FileSystem.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace FileSystem;


// ²âÊÔÊäÈëÑ¡Ïî£º
// FileCatalogue.exe ..\FileCatalogue\    
// FileCatalogue.exe ..\FileCatalogue\ *.cpp *.h /s
// FileCatalogue.exe ..\FileCatalogue\ /d
// FileCatalogue.exe ..\FileCatalogue\ / f main

int main(int argc, char* argv[])
{
	CFileCatalog* fc = CFileCatalog::instance();
	string path = argv[1];
	string absolutePath = Path::getFullFileSpec(path);
	fc->addPath(absolutePath);

	string option;
	if (argc == 2)
	{
		option = "status";
		cout << fc->fileCount() << " Files found in " << fc->dirCount() << " directories" << endl;
		return 0;
	}
	
	string searchText;
	bool isRoot = false;

	option = "search_file";

	for (int i = 0; i < argc; i++)
	{
//		cout << "argv[i]: " << argv[i] << ", line: " << __LINE__ << endl;
		if (strcmp(argv[i], "/s") == 0)
		{
			option = "search_file";
			isRoot = true;
		}
		else if (strcmp(argv[i], "/d") == 0)
		{
			option = "remove";
		}
		else if (strcmp(argv[i], "/f") == 0)
		{
			option = "search_text";
			searchText = argv[i + 1];
		}
	}

	if (option == "search_file")
	{
		vector<string> pattern;
		pattern.push_back(argv[2]);
		pattern.push_back(argv[3]);
		vector<string> f = fc->findFile(absolutePath, isRoot, pattern);
		cout << "size: " << f.size() << ", line: " << __LINE__ << endl;
		for (size_t i = 0; i < f.size(); i++)
		{
			cout << f[i] << endl;
		}
	}
	else if (option == "remove")
	{
		cout << __FILE__ << __LINE__ << endl;
		fc->removeDuplicatedFile();
		cout << __FILE__ << __LINE__ << endl;
	}
	else if (option == "search_text")
	{
		vector<string> f = fc->searchFile(absolutePath, searchText);
		cout << "size: " << f.size() << ", line:" << __LINE__ << endl;
		
		for (size_t i = 0; i < f.size(); i++)
		{
			cout << f[i] << endl;
		}
	}
	else {

	}

	return 0;
}

//int main(int argc, char* argv)
//{
//	CFileCatalogue* fc = CFileCatalogue::instance();
//
//	string input;
//	string cmd;
//	while (true)
//	{
//		getline(cin, cmd);
//		cout << "cmd is " << cmd << " end" << endl;
//		if (cmd == "")
//		{
//			cout << "There are " << fc->fileCount() << " files in " << fc->dirCount() << " directories" << endl;
//			continue;
//		}
//		
//		// use -file cmd to add files to catalog
//		// -file \dir\file1 \dir2\file
//		if (cmd.find("-file") != string::npos)
//		{
//			size_t pos = cmd.find("-file");
//			cout << "-file" << endl;
//			size_t pos1 = 0;
//			size_t pos2 = 0;
//			while (true)
//			{
//				pos1 = cmd.find(' ', pos);
//				cout << "pos1 = " << pos1 << endl;
//				pos2 = cmd.find(' ', pos1 + 1);
//				cout << "pos2 = " << pos2 << endl;
//				if (pos2 == string::npos)
//				{
//					string fileName = cmd.substr(pos1 + 1);
//					cout << fileName << " end" << endl;
//					if (fc->addFile(fileName) == false)
//					{
//						cout << "there is a file with same name in the same directory" << endl;
//					}
//					break;
//				}
//				else {
//					string fileName = cmd.substr(pos1 + 1, pos2 - pos1 - 1);
//					cout << fileName << " end" << endl;
//					if (fc->addFile(fileName) == false)
//					{
//						cout << "there is a file with same name in the same directory" << endl;
//					}
//				}
//				pos = pos2;
//			}
//		}
//		
//		// use -find cmd to find files match the patterns
//		// -find -s -path \dir1\ -pattern *.txt aaa.*
//		if (cmd.find("-find") != string::npos)
//		{
//			string path;
//			{
//				size_t pos1 = cmd.find("-path");
//				size_t pos2 = cmd.find(' ', pos1);
//				size_t pos3 = cmd.find(' ', pos2 + 1);
//				path = cmd.substr(pos2 + 1, pos3 - pos2 - 1);
//				cout << "path is " << path << endl;
//			}
//			vector<string> patterns;
//			{
//				size_t pos = cmd.find("-pattern");
//				cout << "-pattern" << endl;
//				size_t pos1 = 0;
//				size_t pos2 = 0;
//				while (true)
//				{
//					pos1 = cmd.find(' ', pos);
//					cout << "pos1 = " << pos1 << endl;
//					pos2 = cmd.find(' ', pos1 + 1);
//					cout << "pos2 = " << pos2 << endl;
//					if (pos2 == string::npos)
//					{
//						string p = cmd.substr(pos1 + 1);
//						cout << p << " end" << endl;
//						patterns.push_back(p);
//						break;
//					}
//					else {
//						string p = cmd.substr(pos1 + 1, pos2 - pos1 - 1);
//						cout << p << " end" << endl;
//						patterns.push_back(p);
//					}
//					pos = pos2;
//				}
//				for (size_t i = 0; i < patterns.size(); ++i)
//				{
//					cout << "i = " << i << ", " << patterns[i] << endl;
//				}
//			}
//			bool isRoot = false;
//			if (cmd.find("-s") != string::npos)
//			{
//				isRoot = true;
//			}
//			vector<string> rt = fc->findFile(path, isRoot, patterns);
//			cout << "find files match patterns: " << endl;
//			for (size_t i = 0; i < rt.size(); ++i)
//			{
//				cout << rt[i] << endl;
//			}
//		}
//
//		// use /d to emit duplicate file names
//		if (cmd.find("-delete") != string::npos)
//		{
//			fc->deleteDuplicatedFile();
//		}
//
//	}
//}