
#pragma once

#include "IFileParser.hpp"

std::shared_ptr<IFileParser> FileParserRegister(std::shared_ptr<IFileParser> parser);
template <class Derived, bool SelfRegister> class FileParser : public IFileParser
{
    static inline std::shared_ptr<IFileParser> Registered = FileParserRegister(std::make_shared<Derived>());
  protected:
    FileParser(const FileParserInfo &info) : IFileParser(info)
    {
    }
    private:
    static bool Register()
    {
      LASSERT(!Registered,"Already Registered");
      Registered = FileParserRegister(std::make_shared<Derived>());
    }
};
