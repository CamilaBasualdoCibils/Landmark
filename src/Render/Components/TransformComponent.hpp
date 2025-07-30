#pragma once
#include <pch.h>
#include "Render/Types/Transform.hpp"
#include "Render/Components/IComponent.hpp"
 struct TransformComponent : IComponent, Transform
{
    TransformComponent(EntityID ID) : IComponent(ID)
    {
    }
        void DrawInspector()
    {
        Editor::DragVec3("Position", &Position, 0.1f);
        Editor::Rotation("Rotation", &Rotation);
        Editor::DragVec3("Scale", &Scale, 0.1f);
    }
};