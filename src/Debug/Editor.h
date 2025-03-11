#pragma once
#include "EditorObjectGroup.h"
#include "imgui/imgui.h"


class  Editor
{

	//    Priority | toolset
	//std::map<>
	static inline EditorObjectGroup toolGroups = EditorObjectGroup("Main");




public:
	static void Init();
	static void Draw();
	static EditorObjectGroup& GetMainToolGroup() { return toolGroups; }

};

