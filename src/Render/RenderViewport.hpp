#pragma once
#include <Editor/EditorToolItem.hpp>
#include <Editor/EditorWindow.hpp>
#include <pch.h>
class RenderViewport : public Editor::ToolItem, EditorWindow<>
{
    bool Open = true;
    void DrawWindowContents() override;
    void DrawTool() override
    {
        if (Open)
            EditorWindow<>::Draw();
    }
    void DrawHandle() override
    {
        ImGui::MenuItem(GetLabel().c_str(), nullptr, &Open);
    }
    public:
    RenderViewport():Editor::ToolItem("Viewport"),EditorWindow<>("Viewport")
    {
        //AllowVerticalScrollBar(false);
    }
};