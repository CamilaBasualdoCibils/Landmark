#include "Entity.hpp"
#include "TransformComponent.hpp"
#include "EntityInfo.hpp"
#include "Render/World.hpp"
#include "EntityInfo.hpp"
Entity::Entity(EntityID ID) : ID(ID)
{
    AddOrGetComponent<TransformComponent>();
    AddOrGetComponent<EntityInfo>();
}

EntityInfo &Entity::GetInfo()
{
    return GetComponent<EntityInfo>();
}

const EntityInfo &Entity::GetInfo() const
{
    return GetComponent<const EntityInfo>();
}

TransformComponent &Entity::GetTransform()
{
    return GetComponent<TransformComponent>();
}

const TransformComponent &Entity::GetTransform() const
{
    return GetComponent<const TransformComponent>();
}

entt::registry& Entity::GetRegistry()
{
    return World::Get().GetRegistry();
}