#include "pch.h"
#include "Folder.h"
#include <filesystem>

#include "File.h"

Folder::Folder(const std::string& path) : FileSystemObject(path)
{
}

bool Folder::Exists() const
{
	return std::filesystem::exists(GetPath()) && std::filesystem::is_directory(GetPath());
}

bool Folder::Create()
{
	if (Exists()) return false;

	try {
		std::filesystem::create_directory(GetPath());
		return true;
	}
	catch (const std::filesystem::filesystem_error& e) {
		LOGGER.Error("Error creating directory: " + std::string(e.what()));
		//LOGGER.Error() << "Error creating directory: " << e.what() << Log::end;
		return false;
	}

}

bool Folder::Erase()
{
	if (Exists()) {
		try {
			std::filesystem::remove_all(GetPath());
			return true;
		}
		catch (const std::filesystem::filesystem_error& e) {
			LOGGER.Error("Error removing directory: " + std::string(e.what()));
			//LOGGER.Error() << "Error removing directory: " << e.what() << Log::end;
			return false;	
		}
	}
	return false;
}

bool Folder::EraseContents()
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
		LOGGER.Error("Error erasing folder contents: " + std::string(e.what()));
		//LOGGER.Error() << "Error erasing folder contents: " << e.what() << Log::end;
		return false;
	}
}

File Folder::GetFile(const std::string& file)
{
	return File(std::string(GetPath() + "/" + file));
}

const File Folder::GetFile(const std::string& file) const
{
	return GetFile(file);
}

Folder Folder::GetFolder(const std::string& folder)
{
	return Folder(GetPath() + "/" + folder);
}

const Folder Folder::GetFolder(const std::string& folder) const
{
	return GetFolder(folder);
}

const std::vector< Folder> Folder::GetSubFolders() const
{
	if (!Exists())
		return {};

	std::vector<Folder> SubFolders;
		for (const auto& entry : std::filesystem::directory_iterator(GetPath()))
		{
			if (std::filesystem::is_directory(entry))
				SubFolders.push_back(Folder(entry.path().string()));
		}
		return SubFolders;
}

std::vector<Folder> Folder::GetSubFolders()
{
	if (!Exists())
		return {};

	std::vector<Folder> SubFolders;
		for (const auto& entry : std::filesystem::directory_iterator(GetPath()))
		{
			if (std::filesystem::is_directory(entry))
				SubFolders.push_back(Folder(entry.path().string()));
		}
		return SubFolders;
}

const std::vector< File> Folder::GetFiles() const
{
	return GetFiles();
}

std::vector<File> Folder::GetFiles()
{
	if (!Exists())
		return {};

	std::vector<File> SubFiles;
	for (const auto& entry : std::filesystem::directory_iterator(GetPath()))
	{
		if (std::filesystem::is_regular_file(entry))
			SubFiles.push_back(File(entry.path().string()));
	}
	return SubFiles;
}

bool Folder::isEmpty() const
{
    return std::filesystem::is_empty(GetPath());
}
