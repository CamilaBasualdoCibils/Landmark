#pragma once

#include "Object.hpp"
#include "Payload.hpp"
#include <pch.h>
namespace Render
{
namespace Composite
{
class Stack;
class Layer : public Object
{
    friend Stack;
    bool Enabled = true;
    std::string Name;
  public:
    Layer(std::string_view Name)
    {
        SetName(Name);
    }

    void SetName(std::string_view newName)
    {
        Name = newName;
    }
    const std::string& GetName() const
    {
        return Name;
    }
};
} // namespace Composite

} // namespace Render
