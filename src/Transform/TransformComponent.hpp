#pragma once
#include <pch.h>
#include <ECS/Base/ComponentData.hpp>
#include "Transform.h"
class TransformComponent : IComponentData, Transform
{

    // Transform transform;
public:
    // Transform& GetTransform() {return transform;}
    // const Transform& GetTransform() const {return transform;}
    TransformComponent(SceneID scene, ObjectID id) : IComponentData(scene, id)
    {
    }
    static Component_Info GetComponentTypeInfo()
    {
        Component_Info info;
        info.Name = "Transform";
        return info;
    }
    void DrawInspector() override
    {

        vec3 pos = Position();
        if (ImGui::DragFloat3("Position", &pos[0]))
            Position() = pos;

        quat rot = Rotation();
        if (ImGui::DragFloat4("Rotation", &rot[0]))
            Rotation() = rot;
        vec3 euler = eulerAngles(rot);
            if (ImGui::DragFloat3("Rotation Euler", &euler[0]))
                Rotation() = quat(euler);

        vec3 scale = Scale();
        if (ImGui::DragFloat3("Scale", &scale[0]))
            Scale() = scale;
    }
};