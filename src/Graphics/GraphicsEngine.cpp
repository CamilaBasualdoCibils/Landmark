#include "GraphicsEngine.hpp"
#include "Graphics/OpenGL/Commands/CommandManager.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/CommandManager.hpp"

#include "ImGui.hpp"

#include <Graphics/Devices/GPUSelector.hpp>
#include <Graphics/ImGui.hpp>
#include <Graphics/Vulkan/Instance.hpp>
GraphicsEngine::GraphicsEngine(const GraphicsEngineProperties &properties)
{
    VK::InstanceProperties VulkanProperties;
    VK::Instance::Make(VulkanProperties);

    MainGPU = std::make_shared<GPUInstance>();

    MainGPU->SetDevice(GPUSelector::Get().AllDevices()[0]).Init();

    DearImGui::Make(DearImGuiProperties{});
}

void GraphicsEngine::Push(const std::vector<std::shared_ptr<Graphics::ICommandManager>> &CmdBuffers)
{
    for (const auto &b : CmdBuffers)
    {
        ExecuteCommandBuffers.push(b);
    }
}
void GraphicsEngine::ExecuteNow(const std::vector<std::shared_ptr<Graphics::ICommandManager>> &CmdBuffers)
{
    for (const auto &b : CmdBuffers)
    {
        Dispatch(b);
    }
}
void GraphicsEngine::Dispatch(std::shared_ptr<Graphics::ICommandManager> CmdBuffer)
{
    if (const auto OpenGLCommandBuffer = std::dynamic_pointer_cast<GL::CommandManager>(CmdBuffer)) // OPENGL
    {
        OpenGLCommandBuffer->Submit();
    }
    else // VULKAN
    {
        const auto VulkanCommandBuffer = std::dynamic_pointer_cast<VK::CommandManager>(CmdBuffer);
        VulkanCommandBuffer->Submit(MainGPU->VK()->GraphicsQueue());
    }
}
void GraphicsEngine::BeginFrame()
{
    glfwPollEvents();
    DearImGui::Get().BeginFrame();
}
void GraphicsEngine::EndFrame()
{
    DearImGui::Get().EndFrame();
}

void GraphicsEngine::Render()
{
    for (const auto& Window : ActiveWindows)
    {
        Window.second->Render();
    }
    //ExecuteCommandBuffers.push(DearImGui::Get().Render());
    while (!ExecuteCommandBuffers.empty())
    {
        auto CommandBuffer = ExecuteCommandBuffers.front();
        ExecuteCommandBuffers.pop();
        Dispatch(CommandBuffer);
    }

    for (const auto& Window : ActiveWindows)
    {
        Window.second->Present();
    }
    //std::cerr << "Presented\n";
}   