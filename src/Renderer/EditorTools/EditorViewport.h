#pragma once

#include "Debug/EditorTool.h"
#include "Entity/IComponent.h"

class EditorViewport : public EditorTool
{
public:
	EditorViewport() : EditorTool("Viewport")
	{
	}

private:
	bool CameraAssigned = false;
	EntityID CameraSelected = -1;
	bool ViewportOpen = true;
public:

	void DrawHandle() override;

protected:
	void Draw() override;


public:
	void DrawTool() override;
private:
	void DrawNoCameraNotif() const;
};
