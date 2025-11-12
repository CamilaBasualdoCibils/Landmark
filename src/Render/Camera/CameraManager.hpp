#pragma once
#include <pch.h>
#include "misc/Singleton.hpp"
#include "Entity/Entity.hpp"
class CameraManager : public Singleton<CameraManager>
{
    mutable std::optional<EntityID> MainCameraID;
    public:
    CameraManager();

    void SetMainCamera(EntityID id);
    std::optional<EntityID> GetMainCamera();

    
};