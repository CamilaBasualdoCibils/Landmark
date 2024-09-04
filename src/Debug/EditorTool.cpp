#include "pch.h"
#include "EditorTool.h"

EditorTool::EditorTool(const std::string& name) :EditorObject(name)
{

}



void EditorTool::DrawTool()
{
	if (ImGui::Begin(GetName().c_str()))
	{
		Draw();
		
	}
	ImGui::End();

	
}
