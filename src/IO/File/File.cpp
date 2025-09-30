#include "File.hpp"
#include "Folder.hpp"
#include "pch.h"
const_File::const_File(const std::string &_File) : FileSystemObject(_File)
{
}
File::File(const std::string &_File) : const_File(_File)
{
}

bool const_File::Exists() const
{
    return std::filesystem::exists(GetPath()) && std::filesystem::is_regular_file(GetPath());
}

std::string const_File::GetFullExtension() const
{
    const auto pos_of_dot = GetPath().find_first_of('.');
    return GetPath().substr(pos_of_dot + 1);
}

std::string const_File::GetLastExtension() const
{
    const auto pos_of_dot = GetPath().find_last_of('.');
    return GetPath().substr(pos_of_dot + 1);
}

void const_File::ReadBinary(std::vector<std::byte> &binary) const
{
    if (!Exists())
        return;
    std::ifstream file(GetPath(), std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    binary.resize(fileSize);

    file.seekg(0);
    file.read(reinterpret_cast<char *>(binary.data()), fileSize);
    file.close();
}

void const_File::ReadText(std::string &outText) const
{
    if (!Exists())
        throw std::runtime_error("failed to open file!");

    std::ifstream inFile(GetPath());
    if (!inFile)
    {
        std::cerr << ("Failed to Open file (" + GetPath() + ") for reading!") << std::endl;
        return;
    }
    outText.empty();
    std::stringstream buf;
    buf << inFile.rdbuf();
    outText = buf.str();
    // std::string content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();
}

void const_File::ReadJson(Json &outJson) const
{
    std::string text;
    ReadText(text);
    outJson.empty();
    outJson.parse(text);
}

bool File::WriteBinary(const std::vector<std::byte> &data)
{
    std::ofstream file(GetPath(), std::ios::binary);
    if (!file)
    {
        std::cerr << ("Failed to open file for binary writing: " + GetPath()) << std::endl;
        return false;
    }
    file.write(reinterpret_cast<const char *>(data.data()), data.size());
    return file.good();
}

bool File::WriteText(const std::string &text)
{
    std::ofstream file(GetPath());
    if (!file)
    {
        std::cerr << ("Failed to open file for text writing: " + GetPath()) << std::endl;
        return false;
    }
    file << text;
    return file.good();
}

bool File::Move(const std::string &NewDir, std::optional<std::string> NewName)
{
    try
    {
        std::filesystem::rename(GetPath(), NewDir + NewName.value_or(GetName()));
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

bool File::Move(const Folder &targetFolder, std::optional<std::string> NewName)
{
    return Move(targetFolder.GetPath(), NewName);
}

bool File::Copy(Folder targetFolder, std::optional<std::string> NewName, FileCopyOptions copyMethod)
{
    std::error_code errorCode;
    const std::string source = GetPath(), dest = targetFolder.GetPath() + "/" + NewName.value_or(GetName());
    std::filesystem::copy_file(source, dest, (std::filesystem::copy_options)copyMethod, errorCode);
    LASSERT(!errorCode, "Fuuuuuck");
    return true;
}

std::filesystem::file_time_type File::GetLastWriteTime() const
{
    if (!Exists())
        throw "Invalid File";
    return std::filesystem::last_write_time(GetPath());
}

bool const_File::PathEndsWith(const std::string &end) const
{
    return GetPath().size() >= end.size() && GetPath().rfind(end) == (GetPath().size() - end.size());
}
