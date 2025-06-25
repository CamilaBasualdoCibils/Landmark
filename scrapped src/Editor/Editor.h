#pragma once
#include "EditorObjectGroup.h"
#include "imgui/imgui.h"
#include "EditorTopBar.hpp"
#include <misc/Singleton.hpp>
class Console;
class Editor  : public Singleton<Editor>
{

	//    Priority | toolset
	// std::map<>
	EditorTopBar topbar;
	

public:
	Editor();
	static std::shared_ptr<Editor> GetInstance();
	void Draw();
	EditorTopBar &GetMainToolGroup() { return topbar; }
};
