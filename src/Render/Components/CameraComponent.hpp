#pragma once
#include "Render/CameraRenderer.hpp"
#include "Render/Components/IComponent.hpp"
#include "Render/Types/Camera.hpp"
struct CameraComponent : IComponent, Camera
{
    std::shared_ptr<CameraRenderer> cameraRenderer;

    CameraComponent(EntityID ID);

    mat4 GetViewMatrix() const;
};
