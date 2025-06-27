#include "GraphicsEngine.hpp"
#include "Graphics/OpenGL/CommandBuffer/CommandBuffer.hpp"

#include <Graphics/Devices/GPUSelector.hpp>
#include <Graphics/Vulkan/Instance.hpp>
#include <memory>
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

void GraphicsEngine::Execute(const std::vector<std::shared_ptr<ICommandBuffer>> &CmdBuffers)
{
    for (const auto& CommandBuffer : CmdBuffers)
    {
        if (auto OpenGLCommandBuffer = std::dynamic_pointer_cast<GL::CommandBuffer>(CommandBuffer)) //OPENGL
        {
            OpenGLCommandBuffer->Submit();
        }
        else //VULKAN 
        { 
        
        }
    }
}