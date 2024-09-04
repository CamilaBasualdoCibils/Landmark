#pragma once
#include <filesystem>
#include <string>

#include "Debug/Debug.h"



class Resource;
class Folder;
class File
{
	friend Resource;
	friend Folder;


	File(const std::string& _File);


	const std::string FilePath;
	static inline Log LOGGER = Log("Resource>File");

public:

	bool Exists() const;

	std::string GetPath() const;
	std::string GetName() const;

	std::vector<char> GetContent_Binary() const;
	std::string GetContent_Text() const;
};

