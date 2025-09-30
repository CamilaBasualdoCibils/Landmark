#include "pch.h"
#include "Folder.hpp"
#include "File.hpp"

const_Folder::const_Folder(const std::string &path): FileSystemObject(path)

{
}
Folder::Folder(const std::string& path) : const_Folder(path)
{
}

bool const_Folder::Exists() const
{
	return std::filesystem::exists(GetPath()) && std::filesystem::is_directory(GetPath());
}

bool Folder::Create() const
{
	if (Exists()) return false;

	try {
		std::filesystem::create_directory(GetPath());
		return true;
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << ("Error creating directory: " + std::string(e.what())) << std::endl;
		//LOGGER.Error() << "Error creating directory: " << e.what() << Log::end;
		return false;
	}

}

bool Folder::Erase() const
{
	if (Exists()) {
		try {
			std::filesystem::remove_all(GetPath());
			return true;
		}
		catch (const std::filesystem::filesystem_error& e) {
			std::cerr << ("Error removing directory: " + std::string(e.what())) << std::endl;
			//LOGGER.Error() << "Error removing directory: " << e.what() << Log::end;
			return false;	
		}
	}
	return false;
}

bool Folder::EraseContents() const
{
	if (!Exists())
		return false;

	try {
		for (const auto& entry : std::filesystem::directory_iterator(GetPath())) {
			std::filesystem::remove_all(entry.path());
		}
		return true;
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << ("Error erasing folder contents: " + std::string(e.what())) << std::endl;
		//LOGGER.Error() << "Error erasing folder contents: " << e.what() << Log::end;
		return false;
	}
}

File Folder::GetFile(const std::string& file)
{
	return File(std::string(GetPath() + "/" + file));
}


const_File const_Folder::GetFile(const std::string &file) const
{
	return const_File(std::string(GetPath() + "/" + file));
}

Folder Folder::GetFolder(const std::string& folder)
{
	return Folder(GetPath() + "/" + folder);
}

const_Folder const_Folder::GetFolder(const std::string& folder) const
{
	return const_Folder(GetPath() + "/" + folder);
}

void const_Folder::GetSubFolders(std::vector< const_Folder>& folders) const
{
	if (!Exists())
		return ;
	folders = {};
		for (const auto& entry : std::filesystem::directory_iterator(GetPath()))
		{
			if (std::filesystem::is_directory(entry))
				folders.push_back(Folder(entry.path().string()));
		}

}

void Folder::GetSubFolders(std::vector<Folder>& folders)
{
	if (!Exists())
		return;

	folders = {};
		for (const auto& entry : std::filesystem::directory_iterator(GetPath()))
		{
			if (std::filesystem::is_directory(entry))
				folders.push_back(Folder(entry.path().string()));
		}
}

void const_Folder::GetFiles(std::vector< const_File>& files) const
{
	if (!Exists())
		return;

	files = {};
	for (const auto& entry : std::filesystem::directory_iterator(GetPath()))
	{
		if (std::filesystem::is_regular_file(entry))
			files.push_back(File(entry.path().string()));
	}
}

void Folder::GetFiles(std::vector<File>& files)
{
	if (!Exists())
		return ;

	files = {};
	for (const auto& entry : std::filesystem::directory_iterator(GetPath()))
	{
		if (std::filesystem::is_regular_file(entry))
			files.push_back(File(entry.path().string()));
	}
}

bool Folder::isEmpty() const
{
    return std::filesystem::is_empty(GetPath());
}