#pragma once
#include <pch.h>
#include <ECS/Base/ComponentData.hpp>
#include "Transform.h"
class TransformComponent : IComponentData, public Transform
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
        if (ImGui::DragFloat3("Position", &pos[0], 0.1))
            Position() = pos;

        quat rot = Rotation();

        vec3 euler = degrees(eulerAngles(rot));
        if (ImGui::DragFloat3("Rotation", &euler[0], 0.1))
            Rotation() = quat(radians(euler));
        if (ImGui::TreeNode("Quaternion"))
        {
            if (ImGui::DragFloat4("Rotation quat", &rot[0], 0.01))
                Rotation() = rot;
            ImGui::TreePop();
        }

        vec3 scale = Scale();
        if (ImGui::DragFloat3("Scale", &scale[0], 0.01))
            Scale() = scale;
    }
};