#include "pch.h"
#include "File.h"
#include <fstream>
File::File(const std::string& _File) : FilePath(_File)
{
}

bool File::Exists() const
{
	return std::filesystem::exists(FilePath) && std::filesystem::is_regular_file(FilePath);
}

const std::string& File::GetPath() const
{
	return FilePath;
}

std::string File::GetName() const
{
	return FilePath.substr(FilePath.find_last_of("/")+1);
}

std::string File::GetExtension() const
{
	const auto pos_of_dot = GetPath().find_last_of('.');
    return GetPath().substr();
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

std::vector<std::byte> File::GetContent_Binary() const
{
	if (!Exists()) return {};
	std::ifstream file(FilePath, std::ios::ate | std::ios::binary);

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

std::string File::GetContent_Text() const
{
	if (!Exists()) return {};

	std::ifstream inFile(FilePath);
	if (!inFile)
	{
		LOGGER.Error( "Failed to Open file (" + FilePath+ ") for reading!" );
		return {};
	}
	std::string content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();
	return content;


}
