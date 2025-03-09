#pragma once
#include <pch.h>
#include <ECS/Base/ComponentData.hpp>
#include "../Camera.h"
struct CameraComponent : IComponentData,Camera
{
private:
    

public:
    CameraComponent(SceneID scene, ObjectID id) : IComponentData(scene, id) {
    }
    void DrawInspector() override;
    const mat4& GetViewMatrix();
    static Component_Info GetComponentTypeInfo() {
        Component_Info info;
        info.Name = "Camera Component";
        return info;
    }
};