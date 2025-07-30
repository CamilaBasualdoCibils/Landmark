#pragma once
#include "EditorToolItem.hpp"
#include <pch.h>
namespace Editor
{
class LambdaItem : public ToolItem
{
    std::function<void()> HandleLambda, ToolLambda;

  public:
    LambdaItem(const std::string &Name, std::function<void()> HandleLambda, std::function<void()> ToolLambda = nullptr)
        : ToolItem(Name), HandleLambda(HandleLambda), ToolLambda(ToolLambda)
    {
    }

    virtual void DrawTool()
    {
        if (ToolLambda)
            ToolLambda();
    }
    virtual void DrawHandle()
    {
        HandleLambda();
    }
};
} // namespace Editor
