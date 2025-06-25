#include "GraphicsEngine.hpp"
#include "IO/Window/WindowManager.hpp"
#include <Graphics/Devices/GPUSelector.hpp>
#include <Graphics/Vulkan/Instance.hpp>
GraphicsEngine::GraphicsEngine(const GraphicsEngineProperties &properties)
{
    VK::InstanceProperties VulkanProperties;
    VK::Instance::Make(VulkanProperties);

    MainGPU = std::make_shared<GPUInstance>();

    MainGPU->SetDevice(GPUSelector::Get().AllDevices()[0]).Init();
}
std::shared_ptr<GL::CommandBuffer> GraphicsEngine::MakeGLCommandBuffer()
{
    return std::make_shared<GL::CommandBuffer>();
};