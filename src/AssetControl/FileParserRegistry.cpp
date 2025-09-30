#include "FileParserRegistry.hpp"

void FileParserRegistry::Register(std::shared_ptr<IFileParser> parser)
{
FileParsers.push_back(parser);

}