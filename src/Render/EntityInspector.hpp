#pragma once
#include <pch.h>
#include "Editor/EditorToolItem.hpp"
#include "Editor/EditorWindow.hpp"
class EntityInspector : public Editor::ToolItem, EditorWindow<>
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
    EntityInspector() : Editor::ToolItem("Entity Inspector"), EditorWindow<>("Entity Inspector")
    {
    }
};