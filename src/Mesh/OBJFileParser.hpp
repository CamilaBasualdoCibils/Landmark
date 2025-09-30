#pragma once
#include "pch.h"
#include "AssetControl/FileParser.hpp"

class ObjFileParser : FileParser<ObjFileParser,true>
{

    public:
    ObjFileParser();
    private:
    bool TryParse(const std::unordered_multimap<FileParserExtension, const_File> &InFiles,
                          std::vector<AssetCreationInfo>& outAssets,std::unordered_multimap<FileParserExtension, const_File>& FilesParsed) const override;
};