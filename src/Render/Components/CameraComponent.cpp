#include "CameraComponent.hpp"
#include "Render/CameraRenderer.hpp"
#include "TransformComponent.hpp"
CameraComponent::CameraComponent(EntityID ID):IComponent(ID)
{
    compositeGroup->AddLayer<CameraRenderer>(GetEntity());
}

mat4 CameraComponent::GetViewMatrix() const
{
    return Camera::GetViewMatrix(GetEntity().GetComponent<TransformComponent>());
}
