#include "FileSystemObject.hpp"
#include "Folder.hpp"
FileSystemObject::FileSystemObject(const std::string &_path) :path(_path) 
{
}

const std::string &FileSystemObject::GetPath() const
{
    return path;
}

std::string FileSystemObject::GetName() const
{
    return path.substr(path.find_last_of("/")+1);
}

std::string FileSystemObject::GetParentPath() const
{
    std::string dir = path;
    dir = dir.substr(0,path.find_last_of("/"));
    return dir;
}

Folder FileSystemObject::GetParentFolder() const
{
    
    return Folder(GetParentPath());
}

bool FileSystemObject::isFolder() const
{
    return std::filesystem::is_directory(GetPath());
}

bool FileSystemObject::isFile() const
{
    return std::filesystem::is_regular_file(GetPath());
}