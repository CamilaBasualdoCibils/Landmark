#pragma once
#include "IComponent.hpp"
#include <pch.h>
struct EntityInfo : IComponent
{
    EntityInfo(EntityID ID) : IComponent(ID)
    {
    }
    std::string Name = "Default Name";
    bool Enabled = true;
    bool Locked = false; //Can the entity be modified
    bool Hidden = false; //Can the entity be listed. If it is hidden then it only exists to the one who hid it.
};
