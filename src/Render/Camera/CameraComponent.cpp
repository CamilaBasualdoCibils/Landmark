#include "CameraComponent.hpp"
#include "Entity/Entity.hpp"
#include "Transform/TransformComponent.hpp"
CameraComponent::CameraComponent(EntityID ID):IComponent(ID)
{
    //compositeGroup->AddLayer<GBufferRenderer>(GetEntity());
    //compositeGroup->AddLayer<CameraRenderer>(GetEntity());
}

mat4 CameraComponent::GetViewMatrix() const
{
    return Camera::GetViewMatrix(GetEntity().GetComponent<TransformComponent>());
}
