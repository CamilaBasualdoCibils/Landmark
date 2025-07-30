#pragma once
#include <pch.h>
#include "EntityCompDefines.hpp"
class Entity;
struct IComponent
{
    const EntityID ID;
    IComponent(EntityID ID) : ID(ID)
    {
    }
    virtual ~IComponent() {};
    virtual void DrawInspector()
    {
    }
    Entity GetEntity() const;
};