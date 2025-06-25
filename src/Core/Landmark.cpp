#include "Landmark.hpp"
#include "IO/Window/WindowManager.hpp"
#include <Graphics/Vulkan/Instance.hpp>
#include <Graphics/Devices/GPUSelector.hpp>
#include <Graphics/GraphicsEngine.hpp>
Landmark::Landmark(const LandmarkStartProperties &Properties)
{

    GraphicsEngineProperties props;
    GraphicsEngine::Make(props);

     //auto GLCommandBuffer = GraphicsEngine::Get().MakeGLCommandBuffer();
    while (!WindowManager::Get().GetMainWindow()->GetShouldClose())
    {
        WindowManager::Get().Update();
    }
}
