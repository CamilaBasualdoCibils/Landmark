#pragma once
#include "EditorToolBar.hpp"
#include <misc/Singleton.hpp>
namespace Editor
{
class EditorMainMenuBar : public ToolBar
{
  public:
    EditorMainMenuBar() : ToolBar("Main Menu Bar")
    {
    }
    void DrawHandle() override
    {
        if (ImGui::BeginMainMenuBar())
        {
            for (const auto &tool : GetTools())
            {
                tool.second->DrawHandle();
            }
            ImGui::EndMainMenuBar();
        }
    }
};
class Editor : public Singleton<Editor>
{

    //    Priority | toolset
    // std::map<>
    EditorMainMenuBar topbar;

  public:
    Editor();
    void Draw();
    ToolBar &GetMainToolGroup()
    {
        return topbar;
    }
    const std::string File_ToolBar = "File", Edit_ToolBar = "Edit", Debug_ToolBar = "Debug", View_ToolBar = "View",Tool_ToolBar = "Tool";
};

} // namespace Editor
