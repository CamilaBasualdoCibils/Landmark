#pragma once
#include <pch.h>
namespace Editor
{
class Widget
{

    const std::string Name;




  public:
      Widget(const std::string &_name) : Name(_name)
    {
    }
    const std::string &GetLabel() const
    {
        return Name;
    };

    virtual void Draw() = 0;

};

} // namespace Editor
