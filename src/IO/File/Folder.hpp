#pragma once
#include "FileSystemObject.hpp"
class File;
class const_File;
class const_Folder : public FileSystemObject
{

  public:
    const_Folder(const std::string &path);
    const_Folder &operator=(const const_Folder &) = default;
    // generic
    bool Exists() const;

    const_File GetFile(const std::string &file) const;
    const_Folder GetFolder(const std::string &folder) const;

    void GetSubFolders(std::vector<const_Folder> &folders) const;
    void GetFiles(std::vector<const_File> &files) const;
    bool isEmpty() const;
};
class Folder : public const_Folder
{

  public:
    Folder(const std::string &path);
    Folder &operator=(const Folder &) = default;
    // generic
    bool Create() const;
    bool Erase() const;
    bool EraseContents() const;
    File GetFile(const std::string &file);
    Folder GetFolder(const std::string &folder);
    void GetSubFolders(std::vector<Folder> &folders);
    void GetFiles(std::vector<File> &files);

    bool isEmpty() const;
};