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
bool open = false;
	EditorTool(const std::string& name);

	virtual void Draw() = 0;


public:
	void DrawTool() override;
	void DrawHandle() override;
	void SetIsOpen(bool o) {open = o;}
	constexpr bool GetIsOpen() const {return open;}
};