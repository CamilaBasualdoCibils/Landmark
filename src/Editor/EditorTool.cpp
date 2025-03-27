#include "pch.h"
#include "EditorTool.h"

EditorTool::EditorTool(const std::string& name) :EditorObject(name)
{

}



void EditorTool::DrawTool()
{
	if (!open) return;
	if (ImGui::Begin(GetName().c_str(),&open))
	{
		Draw();
		
	}
	ImGui::End();

	
}

void EditorTool::DrawHandle()
{
	ImGui::MenuItem(GetName().c_str(), 0, &open);
}
