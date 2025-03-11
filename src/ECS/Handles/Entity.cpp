#include "Entity.hpp"
#include <Core/App.h>
#include <ECS/Scene.hpp>
#include <ECS/Base/EntityData.hpp>
EntityData *Entity::operator->()
{
    LASSERT(isValid(),"Invalid Entity");
    return GetScene()->GetEntityData(GetObjectID());
}

EntityData &Entity::operator*()
{
    return *operator->();
}
