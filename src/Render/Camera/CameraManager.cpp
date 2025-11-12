#include "CameraManager.hpp"
#include "Entity/World.hpp"
#include "Render/Camera/CameraComponent.hpp"
CameraManager::CameraManager()
{
}

void CameraManager::SetMainCamera(EntityID id)
{
    if (!World::Get().GetComponentView<const CameraComponent>().contains(id))
    {
        MainCameraID.reset();
    }
    MainCameraID.emplace(id);
}

std::optional<EntityID> CameraManager::GetMainCamera()
{
    if (const auto CameraView = World::Get().GetComponentView<const CameraComponent>();
        !MainCameraID.has_value() || !CameraView.contains(MainCameraID.value()))
    {
        if (CameraView.begin() == CameraView.end())
        {
            MainCameraID.reset();
        }
        {
            MainCameraID = CameraView.front();
        }
    }
    return MainCameraID;
}
