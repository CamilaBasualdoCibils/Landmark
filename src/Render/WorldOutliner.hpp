#pragma once
#include <Editor/EditorToolItem.hpp>
#include <Editor/EditorWindow.hpp>
class WorldOutliner : public Editor::ToolItem, EditorWindow<>
{
    bool Open = true;
    std::optional<entt::entity> SelectedEntity;
    void DrawWindowContents() override;

    void DrawTool() override
    {
        if (Open)
            EditorWindow<>::Draw();
    }
    void DrawHandle() override
    {
        ImGui::MenuItem(GetLabel().c_str(),nullptr,&Open);
    }
    public:
    decltype(SelectedEntity) GetSelectedEntity() const {return SelectedEntity;}
    WorldOutliner() :Editor::ToolItem("World Outliner"),EditorWindow<>("World Outliner") {}
};