﻿#pragma once

#include "Editor/EditorTool.h"

#include <Types/Viewport.h>
#include <Renderer/Camera.h>
#include <Transform/Transform.h>
#include <ECS/Handles/Component.hpp>
#include <Renderer/Components/CameraComponent.hpp>
class Renderer;

class EditorViewport : public EditorTool
{
public:
	EditorViewport(Renderer* _renderer);

private:
	bool CameraAssigned = false;
	//ObjectID CameraSelected = -1;

	Renderer * const renderer = nullptr;
	vk::DescriptorSet scene_frame_ds;
	Viewport last_viewport;
	Camera viewport_camera;
 	Transform viewport_camera_transform;
	std::optional<Component<CameraComponent>> override_camera;
	

public:
	const Viewport& getViewport() const {return last_viewport;}

protected:
	void DrawWindowContents() override;


public:
	void DrawTool() override;
private:
	void DrawNoCameraNotif() const;


};
