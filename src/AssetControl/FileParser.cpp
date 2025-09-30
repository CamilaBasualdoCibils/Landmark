#include "FileParser.hpp"
#include "FileParserRegistry.hpp"
std::shared_ptr<IFileParser> FileParserRegister(std::shared_ptr<IFileParser> parser)
{
    FileParserRegistry::Get().Register(parser);
    return parser;
}
