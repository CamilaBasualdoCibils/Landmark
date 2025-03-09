#pragma once
#include <pch.h>
class EditorObject
{

	const std::string Name;

protected:
	EditorObject(const std::string& _name) :Name(_name) {}
public:
	const std::string& GetName() const { return Name; };
	//draw the actual tool(s)
	virtual void DrawTool() = 0;
	//draws the handle(s) to enable or disable this tool at the top of the editor
	virtual void DrawHandle() = 0;
};
