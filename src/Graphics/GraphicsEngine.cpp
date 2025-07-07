#include "GraphicsEngine.hpp"
#include "Graphics/OpenGL/CommandBuffer/CommandBuffer.hpp"
#include "Graphics/Vulkan/CommandBuffer/CommandBuffer.hpp"
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

void GraphicsEngine::Push(const std::vector<GPURef<ICommandBuffer>> &CmdBuffers)
{
    for (const auto &b : CmdBuffers)
    {
        ExecuteCommandBuffers.push(b);
    }
}
void GraphicsEngine::ExecuteNow(const std::vector<GPURef<ICommandBuffer>> &CmdBuffers)
{
    for (const auto &b : CmdBuffers)
    {
        Dispatch(b);
    }
}
void GraphicsEngine::Dispatch(GPURef<ICommandBuffer> CmdBuffer)
{
    if (const auto OpenGLCommandBuffer = std::dynamic_pointer_cast<GL::CommandBuffer>(CmdBuffer)) // OPENGL
    {
        OpenGLCommandBuffer->Submit();
    }
    else // VULKAN
    {
        const auto VulkanCommandBuffer = std::dynamic_pointer_cast<VK::CommandBuffer>(CmdBuffer);
        VulkanCommandBuffer->Submit(MainGPU->VK()->GraphicsQueue());
    }
}
void GraphicsEngine::BeginFrame()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void GraphicsEngine::EndFrame()
{
    ImGuiIO &io = ImGui::GetIO();
    ImGui::Render();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void GraphicsEngine::Render()
{
       while (!ExecuteCommandBuffers.empty())
    {
        const auto &CommandBuffer = ExecuteCommandBuffers.front();
        ExecuteCommandBuffers.pop();
        Dispatch(CommandBuffer);
    }
}