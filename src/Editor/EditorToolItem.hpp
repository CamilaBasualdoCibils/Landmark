#pragma once
#include <pch.h>
#include "EditorWidget.hpp"
namespace Editor
{
class ToolItem : public Widget
{

    void Draw() override {DrawTool();}
    public:
    ToolItem(const std::string& Name):Widget(Name) {}
    
    virtual void DrawTool() = 0;
    virtual void DrawHandle() = 0;
};
} // namespace Editor
