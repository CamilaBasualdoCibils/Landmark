#pragma once
#include <filesystem>
#include <string>

#include "Editor/Debug.h"
#include "FileSystemObject.hpp"
class Resource;
class Folder;
class File : public FileSystemObject
{
	friend Resource;
	friend Folder;

	static inline Log LOGGER = Log("File");

public:
	File(const std::string &_File);
	File &operator=(const File &) = default;

	bool Exists() const;

	std::string GetFullExtension() const;
	std::string GetLastExtension() const;

	std::vector<std::byte> ReadBinary() const;
	std::string ReadText() const;
	Json ReadJson() const;

	bool WriteBinary(const std::vector<std::byte> &data);
	bool WriteText(const std::string &text);
	std::filesystem::file_time_type GetLastWriteTime() const;
	bool PathEndsWith(const std::string& end) const;

};
