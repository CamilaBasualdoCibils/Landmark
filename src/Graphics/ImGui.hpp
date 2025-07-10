#pragma once
#include <Graphics/Vulkan/Commands/CommandManager/CommandManager.hpp>
#include <misc/Singleton.hpp>
#include <Graphics/Vulkan/Rendering/RenderTarget.hpp>
#include <pch.h>
struct DearImGuiProperties
{
    // std::shared_ptr<GPUInstance> GPU;
};
class DearImGui : public Singleton<DearImGui, DearImGuiProperties>
{
    ImGui_ImplVulkanH_Window IMGUI_WINDOW;

    GPURef<VK::CommandManager> cmdManager = GPURef<VK::CommandManager>::MakeRef(VK::CmdCapatiblities::eGraphics);


  public:
    DearImGui(const DearImGuiProperties &Properties);
    void BeginFrame();
    void EndFrame();
    [[nodiscard]] GPURef<VK::CommandManager> Render();
};