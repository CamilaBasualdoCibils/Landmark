#pragma once
#include "Debug/EditorTool.h"
#include "Entity/Entity.h"

class EditorEntityList : public EditorTool
{

	bool Open = true;
	bool InspectorOpen = true;
	const std::string InspectorName = "Inspector";
	EntityID entitySelected = -1; 
public:
	EditorEntityList();
	void DrawHandle() override;

protected:
	void Draw() override;


public:
	void DrawTool() override;
private:
	void DrawInspector();
};
