#pragma once
#include "EditorObjectGroup.h"
#include "imgui/imgui.h"
#include "EditorTopBar.hpp"
#include <Core/AppModule.h>
class Editor : public AppModule
{

	//    Priority | toolset
	// std::map<>
	EditorTopBar topbar;

	static inline std::shared_ptr<Editor> INSTANCE;

public:
	Editor();
	static std::shared_ptr<Editor> GetInstance();
	void Init();
	void Draw();
	EditorTopBar &GetMainToolGroup() { return topbar; }
};
