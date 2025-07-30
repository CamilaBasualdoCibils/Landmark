#pragma once
#include <pch.h>
#include "IComponent.hpp"
#include "Render/Mesh.hpp"
struct MeshDescriptor : IComponent
{
    MeshDescriptor(EntityID ID) : IComponent(ID)
    {
    }

    std::shared_ptr<Mesh> mesh;
};
