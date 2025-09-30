#include "OBJFileParser.hpp"

ObjFileParser::ObjFileParser()
    : FileParser<ObjFileParser, true>(
          FileParserInfo{.FileParserName = "OBJ Model Parser", .FileExtensionGroups = {"obj", "mtl"}})
{
}

bool ObjFileParser::TryParse(const std::unordered_multimap<FileParserExtension, const_File> &InFiles,
                             std::vector<AssetCreationInfo> &outAssets,
                             std::unordered_multimap<FileParserExtension, const_File> &FilesParsed) const
{
    return false;
}
