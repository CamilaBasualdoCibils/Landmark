﻿#include "pch.h"
#include "EditorTool.hpp"

EditorTool::EditorTool(const std::string& name) :EditorObject(name),EditorWindow(name)
{

}



void EditorTool::DrawTool()
{
	EditorWindow<>::Draw();
	
}

void EditorTool::DrawHandle()
{
	 bool open = GetIsOpen();
	ImGui::MenuItem(GetLabel().c_str(), 0, &open);
	SetIsOpen(open);
}
