#pragma once
#include <pch.h>
#include "Editor/EditorToolItem.hpp"
#include "Editor/EditorWindow.hpp"
class EntityInspector : public Editor::ToolItem, Editor::Window<>
{
    bool Open = true;
    void DrawWindowContents() override;

    void DrawTool() override
    {
        if (Open)
            Editor::Window<>::Draw();
    }
    void DrawHandle() override
    {
        ImGui::MenuItem(GetLabel().c_str(), nullptr, &Open);
    }

  public:
    EntityInspector() : Editor::ToolItem("Entity Inspector"), Editor::Window<>("Entity Inspector")
    {
    }
};