#include "pch.h"
#include "EditorTool.h"

EditorTool::EditorTool(const std::string& name) :EditorObject(name),EditorWindow(name)
{

}



void EditorTool::DrawTool()
{
	EditorWindow<>::Draw();
	
}

void EditorTool::DrawHandle()
{
	ImGui::MenuItem(GetLabel().c_str(), 0, &open);
}
