#pragma once
#include <pch.h>

#include "IComponent.hpp"

struct MeshRenderer : IComponent
{
    MeshRenderer(EntityID ID) : IComponent(ID) {}
};