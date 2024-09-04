#pragma once
#include "EditorObject.h"
class Editor;
class EditorTool : public EditorObject
{
	friend Editor;

	const uint8_t MaxInstances = 1;
	glm::vec2 Position = { 0,0 };
	glm::vec2 Size = { 0,0 };

	bool Docked = true;
	
protected:
	EditorTool(const std::string& name);

	virtual void Draw() = 0;


public:
	void DrawTool() override;
};