#include "FileCatalogue.h"
#include <assert.h>
#include <stdio.h>
#include <regex>
#include <iostream>
#include <stack>

using namespace FileSystem;
using namespace std;

CFileCatalogue::CFileCatalogue()
{

}

CFileCatalogue::~CFileCatalogue()
{
	map<string, File*>::iterator it;
	for (it = this->files_.begin(); it != this->files_.end(); ++it)
	{
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}
}

CFileCatalogue* CFileCatalogue::instance()
{
	static CFileCatalogue instance__;
	return &instance__;
}

bool CFileCatalogue::addFile(const std::string& fileName)
{
	string path = Path::getPath(fileName);
	cout << path << endl;
	if (path == ".")
	{
		path = "\\";
	}
	else if (path[0] != '\\')
	{
		path = "\\" + path;
	}
	cout << path << endl;
	string name = Path::getName(fileName);
	cout << name << endl;
	string fileSpec = path + name;
	cout << "fileSpec: " << fileSpec << endl;
	map<string, File*>::iterator it;
	it = this->files_.find(fileSpec);
	if (it != this->files_.end())
	{
		map<string, int>::iterator it = this->duplicatedFiles_.find(fileSpec);
		if (it != this->duplicatedFiles_.end())
		{
			this->duplicatedFiles_[fileSpec]++;
		}
		else {
			this->duplicatedFiles_[fileSpec] = 2;
		}
		return false;
	}

	File* pFile = new File(fileName);
	assert(pFile != NULL);
	this->files_[fileName] = pFile;

	this->dirs_[path].push_back(name);
	return true;
}


//bool CFileCatalogue::removeFile(const std::string& fileName)
//{
//	map<string, File*>::iterator it;
//	it = this->files_.find(fileName);
//	if (it == this->files_.end())
//	{
//		return false;
//	}
//	this->files_.erase(it);
//	return true;
//}

int CFileCatalogue::fileCount()
{
	return (int) this->files_.size();
}

int CFileCatalogue::dirCount()
{
	return (int) this->dirs_.size();
}

void CFileCatalogue::deleteDuplicatedFile()
{
	map<string, int>::iterator it;
	int fileCount = 0;
	int allCount = 0;
	for (it = this->duplicatedFiles_.begin(); it != this->duplicatedFiles_.end(); ++it)
	{
		fileCount++;
		allCount += it->second;
	}
	cout << "There are " << fileCount << " duplicated files, and the totoal number is " << allCount << endl;

	this->duplicatedFiles_.clear();
}

vector<string> CFileCatalogue::findFile(const string& path, bool isRoot, const vector<string>& patterns)
{
	vector<string> rt;
	vector<regex> rgx;
	for (size_t i = 0; i < patterns.size(); i++)
	{
		rgx.push_back(regex(patterns[i]));
	}
	if (isRoot == false)
	{
		map<string, list<string> >::iterator it;
		it = this->dirs_.find(path);
		if (it == this->dirs_.end())
		{
			return rt;
		}
		
		list<string>::iterator itList = it->second.begin();
		for (; itList != it->second.end(); ++itList)
		{
			match_results<string::const_iterator> result;
			for (size_t i = 0; i < rgx.size(); i++)
			{
				if (regex_match(*itList, rgx[i]))
				{
					rt.push_back(it->first + *itList);
					break;
				}
			}
		}
		return rt;
	}

	map<string, list<string> >::iterator it = this->dirs_.begin();
	for (; it != this->dirs_.end(); it++)
	{
		if (it->first.find(path) == 0) {
			list<string>::iterator itList = it->second.begin();
			for (; itList != it->second.end(); ++itList)
			{
				match_results<string::const_iterator> result;
				for (size_t i = 0; i < rgx.size(); i++)
				{
					if (regex_match(*itList, rgx[i]))
					{
						rt.push_back(it->first + *itList);
						break;
					}
				}
			}
		}
	}
	return rt;
}

CFileCatalog::CFileCatalog()
{

}


CFileCatalog::~CFileCatalog()
{

}

CFileCatalog* CFileCatalog::instance()
{
	static CFileCatalog instance__;
	return &instance__;
}

bool CFileCatalog::addFile(const std::string& file, const std::string& fileSpec)
{
	map<string, list<string> >::iterator it;
	it = this->files_.find(file);
	if (it == this->files_.end())
	{
		list<string> dirs;
		dirs.push_back(fileSpec);
		this->files_[file] = dirs;
	}
	else{
		it->second.push_back(fileSpec);
	}

	string path = Path::getPath(fileSpec);
	it = this->dirs_.find(path);
	if (it == this->dirs_.end())
	{
		list<string> files;
		files.push_back(file);
		this->dirs_[path] = files;
	}
	else{
		it->second.push_back(file);
	}

	return true;
}

bool CFileCatalog::addPath(const std::string& path)
{
	vector<string> files = Directory::getFiles(path);
	for (size_t i = 0; i < files.size(); i++)
	{
		string fileSpec = path + files[i];
		this->addFile(files[i], fileSpec);
	}

	//map<string, list<string> >::iterator it;
	//for (it = this->files_.begin(); it != this->files_.end(); ++it)
	//{
	//	cout << "file: " << it->first << endl;
	//	list<string>::iterator it_list;
	//	for (it_list = it->second.begin(); it_list != it->second.end(); ++it_list)
	//	{
	//		cout << "path: " << *it_list << endl;
	//	}
	//}
	//cout << endl << endl << endl;
	//for (it = this->dirs_.begin(); it != this->dirs_.end(); ++it)
	//{
	//	cout << "path: " << it->first << endl;
	//	list<string>::iterator it_list;
	//	for (it_list = it->second.begin(); it_list != it->second.end(); ++it_list)
	//	{
	//		cout << "file: " << *it_list << endl;
	//	}
	//}
	return true;
}

bool CFileCatalog::removeFile(const std::string& file, const std::string& path)
{
	return true;
}

bool CFileCatalog::removeFile(const std::string& file)
{
	return true;
}

bool CFileCatalog::removeDuplicatedFile()
{
	cout << "line: " << __LINE__ << endl;
	vector<string> files;
	map<string, list<string> >::iterator it;
	for (it = this->files_.begin(); it != this->files_.end(); ++it)
	{
		files.push_back(it->first);
	}

	for (size_t i = 0; i < files.size(); i++)
	{
		list<string> paths = this->files_[files[i]];
		if (paths.size() > 1)
		{
			list<string>::iterator it;
			for (it = paths.begin(); it != paths.end(); ++it)
			{
				string p = *it;
				string cmd = "del " + p + files[i];
				::system(cmd.c_str());
			}
			this->files_.erase(files[i]);
		}
	}

	return true;
}

int CFileCatalog::fileCount()
{
	this->fileCount_ = 0;
	map<string, list<string> >::iterator it;
	for (it = this->dirs_.begin(); it != this->dirs_.end(); ++it)
	{
		this->fileCount_ += (int)(it->second.size());
	}
	return this->fileCount_;
}

int CFileCatalog::dirCount()
{
	this->dirCount_ = (int)this->dirs_.size();
	return this->dirCount_;
}

vector<string> CFileCatalog::findFile(const string& path, bool isRoot, const string& pattern)
{
	vector<string> rt;
	string ext = Path::getExt(pattern);
	
	if (isRoot == false)
	{
		map<string, list<string> >::iterator it;
		it = this->dirs_.find(path);
		if (it == this->dirs_.end())
		{
			return rt;
		}

		list<string>::iterator itList = it->second.begin();
		for (; itList != it->second.end(); ++itList)
		{
			if (Path::getExt(*itList) == ext)
			{
				rt.push_back(it->first + *itList);
			}
		}
		return rt;
	}

	map<string, list<string> >::iterator it = this->dirs_.begin();
	for (; it != this->dirs_.end(); it++)
	{
		if (it->first.find(path) == 0) {
			list<string>::iterator itList = it->second.begin();
			for (; itList != it->second.end(); ++itList)
			{
				if (Path::getExt(*itList) == ext)
				{
					rt.push_back(it->first + *itList);
				}
			}
		}
	}
	return rt;
}

vector<string> CFileCatalog::findFile(const string& path, bool isRoot, const vector<string>& pattern)
{
	vector<string> rt;
	for (size_t i = 0; i < pattern.size(); i++)
	{
		vector<string> r = this->findFile(path, isRoot, pattern[i]);
		for (size_t j = 0; j < r.size(); j++)
		{
			rt.push_back(r[j]);
		}
	}

	return rt;
}

vector<string> CFileCatalog::searchFile(const string& path, const string& text)
{
	vector<string> rt;
	map<string, list<string> >::iterator it;
	it = this->dirs_.find(path);
	if (it == this->dirs_.end())
	{
		return rt;
	}

	list<string> files = it->second;
	list<string>::iterator it2;
	for (it2 = it->second.begin(); it2 != it->second.end(); ++it2)
	{
		string name = path + *it2;
//		cout << "name: " << name << ", line:" << __LINE__ << endl;
		ifstream inFile;
		inFile.open( name );
		string line;
		while (getline(inFile, line))
		{
			if (line.find(text) != string::npos)
			{
				rt.push_back(name);
				break;
			}
		}
	}
	return rt;
}
