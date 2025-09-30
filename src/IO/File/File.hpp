#pragma once
#include "FileSystemObject.hpp"

enum class FileCopyOptions : std::underlying_type_t<std::filesystem::copy_options>
{
  eOverwrite = (size_t)std::filesystem::copy_options::overwrite_existing,

};
class Folder;
class const_File : public FileSystemObject
{
    friend Folder;

  public:
    const_File() = default;
    const_File(const std::string &_File);
    const_File &operator=(const const_File &) = default;

    bool Exists() const;

    std::string GetFullExtension() const;
    std::string GetLastExtension() const;

    void ReadBinary(std::vector<std::byte> &binary) const;
    void ReadText(std::string &text) const;
    void ReadJson(Json &json) const;

    std::filesystem::file_time_type GetLastWriteTime() const;
    bool PathEndsWith(const std::string &end) const;
};
class File : public const_File
{
    friend Folder;

  public:
    File() = default;
    File(const std::string &_File);
    File &operator=(const File &) = default;

    bool WriteBinary(const std::vector<std::byte> &data);
    bool WriteText(const std::string &text);
    bool Move(const std::string &NewDir, std::optional<std::string> NewName = std::nullopt);
    bool Move(const Folder &targetFolder, std::optional<std::string> NewName = std::nullopt);
    bool Copy(Folder targetFolder,std::optional<std::string> NewName = std::nullopt,FileCopyOptions copyMethod = FileCopyOptions::eOverwrite);
    std::filesystem::file_time_type GetLastWriteTime() const;
};