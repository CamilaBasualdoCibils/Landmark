#pragma once
#include "pch.h"
#include "IO/File/File.hpp"
#include "Asset.hpp"
using FileParserExtension = std::string;

struct FileParserInfo
{
    std::string FileParserName;
    std::vector<FileParserExtension> FileExtensionGroups;
};
class IFileParser
{

    virtual bool TryParse(const std::unordered_multimap<FileParserExtension, const_File> &InFiles,
                          std::vector<AssetCreationInfo> &outAssets,
                          std::unordered_multimap<FileParserExtension, const_File> &FilesParsed) const = 0;

  protected:
    IFileParser(const FileParserInfo &info);

  private:
    FileParserInfo info;
};