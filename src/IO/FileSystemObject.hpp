#pragma once
#include <pch.h>
class Folder;
class FileSystemObject {
    private:
    std::string path;

    public:
    FileSystemObject(const std::string& path);
	const std::string& GetPath() const;
    void SetPath(const std::string& s) {path = s;}
	std::string GetName() const;
    std::string GetParentPath() const;
    Folder GetParentFolder() const;
    bool operator==(const FileSystemObject& o) const {return path == o.path;}
    bool isFolder() const;
    bool isFile() const;

};