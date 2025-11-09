#pragma once

#include "Entity/IComponent.hpp"
#include "Render/Camera/Camera.hpp"
struct CameraComponent : IComponent, Camera
{


    CameraComponent(EntityID ID);

    mat4 GetViewMatrix() const;
};
