#include "CameraComponent.hpp"
#include "Render/CameraRenderer.hpp"
#include "TransformComponent.hpp"
#include "Render/GBufferRenderer.hpp"
CameraComponent::CameraComponent(EntityID ID):IComponent(ID)
{
    compositeGroup->AddLayer<GBufferRenderer>(GetEntity());
    compositeGroup->AddLayer<CameraRenderer>(GetEntity());
}

mat4 CameraComponent::GetViewMatrix() const
{
    return Camera::GetViewMatrix(GetEntity().GetComponent<TransformComponent>());
}
