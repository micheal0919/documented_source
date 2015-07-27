#ifndef FILECATALOGUE_H
#define FILECATALOGUE_H

#include "FileSystem.h"
#include <map>
#include <set>
#include <string>
#include <list>

namespace FileSystem
{
	class CFileCatalogue
	{
	public:
		~CFileCatalogue();

		static CFileCatalogue* instance();

		bool addFile(const std::string& fileName);
//		bool removeFile(const std::string& fileName);
		int fileCount();
		int dirCount();
		void deleteDuplicatedFile();
		std::vector<std::string> findFile(const std::string& path, bool isRoot, const std::vector<std::string>& patterns);
	private:
		CFileCatalogue();
	private:
		std::map<std::string, File*> files_;
		std::map<std::string, std::list<std::string> > dirs_;
		std::map<std::string, int> duplicatedFiles_;
	};

	class CFileCatalog
	{
	public:
		~CFileCatalog();

		static CFileCatalog* instance();

		bool addFile(const std::string& file, const std::string& path);
		bool addPath(const std::string& path);
		bool removeFile(const std::string& file, const std::string& path);
		bool removeFile(const std::string& file);
		bool removeDuplicatedFile();
		int fileCount();
		int dirCount();
		std::vector<std::string> findFile(const std::string& path, bool isRoot, const std::string& patterns);
		std::vector<std::string> findFile(const std::string& path, bool isRoot, const std::vector<std::string>& patterns);
		std::vector<std::string> searchFile(const std::string& path, const std::string& text);
	private:
		CFileCatalog();

	private:
		std::map<std::string, std::list<std::string> > files_;
		std::map<std::string, std::list<std::string> > dirs_;
		int fileCount_;
		int dirCount_;
	};

}

#endif // FILECATALOGUE_H
