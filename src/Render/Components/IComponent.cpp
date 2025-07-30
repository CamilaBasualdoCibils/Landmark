#include "IComponent.hpp"
#include "Entity.hpp"
Entity IComponent::GetEntity() const
{
    return Entity(ID);
}
