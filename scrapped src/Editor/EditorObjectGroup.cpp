#include "pch.h"
#include "EditorObjectGroup.hpp"

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
	if (ImGui::BeginMenu(GetLabel().c_str()))
	{

		for (auto& object : tools)
		{
			object.second->DrawHandle();
		}
		ImGui::EndMenu();
	}

	
}
