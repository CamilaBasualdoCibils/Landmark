#pragma once
#include <string>
#include <vector>

#include "Debug/Debug.h"




class File;
class Folder
{
	
	static inline Log LOGGER = Log("Resource>Folder");
	const std::string Path;
public:
Folder(const std::string& path);
	//generic
	bool Exists() const;
	bool Create();
	bool Erase();
	bool EraseContents();

	File GetFile(const std::string& file);
	const File GetFile(const std::string& file) const;

	Folder GetFolder(const std::string& folder);
	const Folder GetFolder(const std::string& folder) const;

	const std::vector<Folder> GetSubFolders() const;
	std::vector<Folder> GetSubFolders();
	const std::vector< File> GetFiles() const;
	std::vector<File> GetFiles();
};
