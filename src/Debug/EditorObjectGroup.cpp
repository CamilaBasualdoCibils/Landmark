#include "pch.h"
#include "EditorObjectGroup.h"

EditorObjectGroup::EditorObjectGroup(const std::string& _name):EditorObject(_name)
{

}

void EditorObjectGroup::DrawTool()
{
	
	for (auto& object : tools)
	{
		object.second->DrawTool();
	}

	
}

void EditorObjectGroup::DrawHandleWithoutMenu()
{
	for (auto& object : tools)
	{
		object.second->DrawHandle();
	}
}

void EditorObjectGroup::DrawHandle()
{
	if (ImGui::BeginMenu(GetName().c_str()))
	{

		for (auto& object : tools)
		{
			object.second->DrawHandle();
		}
		ImGui::EndMenu();
	}

	
}
