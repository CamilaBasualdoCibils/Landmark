#pragma once
#include "EditorObjectGroup.h"
#include "imgui/imgui.h"


namespace Landmark
{
	class Application;

}

class Renderer;


class EditorTool;

class  Editor
{
	friend Landmark::Application;
	friend Renderer;
	//    Priority | toolset
	//std::map<>
	static inline EditorObjectGroup toolGroups = EditorObjectGroup("Main");

protected:
	static void Init();

	static void Draw();

public:
	static EditorObjectGroup& GetMainToolGroup() { return toolGroups; }

};

