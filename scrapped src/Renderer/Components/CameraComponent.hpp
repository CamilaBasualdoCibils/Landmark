#pragma once
#include <pch.h>
#include <ECS/Base/ComponentData.hpp>
#include "../Camera.h"
static const char CameraCompName[] = "Camera Component";

struct CameraComponent : public LMComponent<CameraComponent,CameraCompName,false>,Camera
{
private:
    

public:
    CameraComponent(SceneID scene, ObjectID id) : LMComponent(scene, id) {
    }
    void DrawInspector() override;
    const mat4& GetViewMatrix();

};

LMCOMPONENT_REGISTER(CameraComponent);