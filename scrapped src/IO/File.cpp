#include "pch.h"
#include "File.h"
#include <fstream>
File::File(const std::string& _File) : FileSystemObject(_File)
{
}

bool File::Exists() const
{
	return std::filesystem::exists(GetPath()) && std::filesystem::is_regular_file(GetPath());
}

std::string File::GetFullExtension() const
{
	const auto pos_of_dot = GetPath().find_first_of('.');
    return GetPath().substr(pos_of_dot+1);
}

std::string File::GetLastExtension() const
{
	const auto pos_of_dot = GetPath().find_last_of('.');
    return GetPath().substr(pos_of_dot+1);
}

std::vector<char> GetBinaryFromFile(std::string Filename)
{
	std::ifstream file(Filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

std::vector<std::byte> File::ReadBinary() const
{
	if (!Exists()) return {};
	std::ifstream file(GetPath(), std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<std::byte> buffer(fileSize);

	file.seekg(0);
	file.read(reinterpret_cast<char*>( buffer.data()), fileSize);
	file.close();

	return buffer;
}

std::string File::ReadText() const
{
	if (!Exists()) throw std::runtime_error("failed to open file!");

	std::ifstream inFile(GetPath());
	if (!inFile)
	{
		LOGGER.Error( "Failed to Open file (" + GetPath()+ ") for reading!" );
		return {};
	}
	std::string content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();
	return content;


}

Json File::ReadJson() const
{
    return Json::parse(ReadText());
}

bool File::WriteBinary(const std::vector<std::byte> &data)
{
	std::ofstream file(GetPath(), std::ios::binary);
	if (!file) {
		LOGGER.Error("Failed to open file for binary writing: " + GetPath());
		return false;
	}
	file.write(reinterpret_cast<const char*>(data.data()), data.size());
	return file.good();
}

bool File::WriteText(const std::string &text)
{
	std::ofstream file(GetPath());
	if (!file) {
		LOGGER.Error("Failed to open file for text writing: " + GetPath());
		return false;
	}
	file << text;
	return file.good();
}

std::filesystem::file_time_type File::GetLastWriteTime() const
{
	if (!Exists()) throw "Invalid File";
    return std::filesystem::last_write_time(GetPath());
}

bool File::PathEndsWith(const std::string &end) const
{
	return GetPath().size() >= end.size() && 
	GetPath().rfind(end) == (GetPath().size() - end.size());
}
