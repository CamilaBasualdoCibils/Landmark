#pragma once
#include <Editor/EditorToolItem.hpp>
#include <Editor/EditorWindow.hpp>
#include "Components/Entity.hpp"
class WorldOutliner : public Editor::ToolItem, Editor::Window<>
{
    bool Open = true;
    std::optional<Entity> SelectedEntity;
    void DrawWindowContents() override;

    void DrawTool() override
    {
        if (Open)
            Editor::Window<>::Draw();
    }
    void DrawHandle() override
    {
        ImGui::MenuItem(GetLabel().c_str(),nullptr,&Open);
    }
    public:
    decltype(SelectedEntity) GetSelectedEntity() const {return SelectedEntity;}
    WorldOutliner() :Editor::ToolItem("World Outliner"),Editor::Window<>("World Outliner") {}
};