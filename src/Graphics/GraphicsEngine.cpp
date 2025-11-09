#include "GraphicsEngine.hpp"
#include "Graphics/OpenGL/Commands/GLCommandManager.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/VKCommandManager.hpp"
#include "Render/Compositor/Compositor.hpp"
#include <Editor/Editor.hpp>
#include <Graphics/Devices/GPUSelector.hpp>
#include <Graphics/Vulkan/VKInstance.hpp>
#include <Platform/Window/WindowManager.hpp>
#include <Platform/DearImGui/DearImGui.hpp>
GraphicsEngine::GraphicsEngine(const GraphicsEngineProperties &properties)
{
    VK::InstanceProperties VulkanProperties;
    VK::Instance::Make(VulkanProperties);

    MainGPU = std::make_shared<GPUInstance>();
    const auto Devices = GPUSelector::Get().AllDevices();
    MainGPU->SetDevice(Devices[0]).Init();

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

        auto FinishedFence = VulkanCommandBuffer->SignalFence();
        VkCommandBuffersInExecution.push_back({FinishedFence, VulkanCommandBuffer});
        VulkanCommandBuffer->Submit(MainGPU->VK()->GraphicsQueue());
    }
}
void GraphicsEngine::BeginFrame()
{
    glfwPollEvents();
    DearImGui::Get().BeginFrame();
    Editor::Editor::Get().Draw();
}
void GraphicsEngine::EndFrame()
{
    DearImGui::Get().EndFrame();

}

void GraphicsEngine::Render()
{
    // Graphics::Compositor::Get().RenderCompositions();
    Render::Compositor::Get().Compose();

    // ExecuteCommandBuffers.push(DearImGui::Get().Render());
    while (!ExecuteCommandBuffers.empty())
    {
        auto CommandBuffer = ExecuteCommandBuffers.front();
        ExecuteCommandBuffers.pop();
        Dispatch(CommandBuffer);
    }
    for (auto it = VkCommandBuffersInExecution.begin(); it != VkCommandBuffersInExecution.end();)
    {
        if (it->first->IsSignaled())
        {
            it = VkCommandBuffersInExecution.erase(it);
        }
        else
        {
            it++;
        }
    }

    Platform::WindowManager::Get().PresentAll();
}