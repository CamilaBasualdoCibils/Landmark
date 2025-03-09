#pragma once
#include <pch.h>
#include "Debug/EditorTool.h"
#include <ECS/ECS_Forward.hpp>
#include <ECS/Handles/Entity.hpp>
class EditorEntityList : public EditorTool
{

	bool Open = true;
	bool InspectorOpen = true;
	const std::string InspectorName = "Inspector";
	Entity entitySelected;
	const std::string entity_create_popup_name = "entity_create_popup";
	std::shared_ptr<SceneManager> scene_manager;
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
