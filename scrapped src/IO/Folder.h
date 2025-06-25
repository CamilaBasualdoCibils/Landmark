#pragma once
#include <string>
#include <vector>

#include "Editor/Debug.h"
#include "FileSystemObject.hpp"

class File;
class Folder : public FileSystemObject
{

	static inline Log LOGGER = Log("Folder");

public:
	Folder(const std::string &path);
	Folder& operator=(const Folder&) = default;
	// generic
	bool Exists() const;
	bool Create();
	bool Erase();
	bool EraseContents();

	File GetFile(const std::string &file);
	const File GetFile(const std::string &file) const;

	Folder GetFolder(const std::string &folder);
	const Folder GetFolder(const std::string &folder) const;

	const std::vector<Folder> GetSubFolders() const;
	std::vector<Folder> GetSubFolders();
	const std::vector<File> GetFiles() const;
	std::vector<File> GetFiles();

	bool isEmpty() const;
};
