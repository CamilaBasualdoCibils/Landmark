#pragma once

#include "Debug/EditorTool.h"

#include <Types/Viewport.h>
class Renderer;
class EditorViewport : public EditorTool
{
public:
	EditorViewport(Renderer* _renderer);

private:
	bool CameraAssigned = false;
	//ObjectID CameraSelected = -1;
	bool ViewportOpen = true;
	Renderer * const renderer = nullptr;
	vk::DescriptorSet scene_frame_ds;
	Viewport last_viewport;
public:

	void DrawHandle() override;
	const Viewport& getViewport() const {return last_viewport;}

protected:
	void Draw() override;


public:
	void DrawTool() override;
private:
	void DrawNoCameraNotif() const;
};
