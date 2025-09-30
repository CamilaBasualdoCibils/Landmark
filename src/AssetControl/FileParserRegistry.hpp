#pragma once
#include "pch.h"
#include "misc/Singleton.hpp"
#include "IFileParser.hpp"
class FileParserRegistry : public Singleton<FileParserRegistry>
{
    using Extension = FileParserExtension;
    using ExtensionGroup = std::vector<Extension>;

    std::vector<std::shared_ptr<IFileParser>> FileParsers = {};
    /** All parsers that can parse a certain extension group */
    std::unordered_multimap<Extension,std::shared_ptr<IFileParser>>GroupParsers = {};

    public:
    FileParserRegistry() = default;
    template <typename T>
    void Register()
    {

        if constexpr (!std::is_base_of_v<IFileParser,T>)
        {
            LASSERT(false,"Invalid Parser");
        }
        std::shared_ptr<T> instance = std::make_shared<T>();
        FileParsers.push_back(instance);
        
    }
    void Register(std::shared_ptr<IFileParser> parser);
};