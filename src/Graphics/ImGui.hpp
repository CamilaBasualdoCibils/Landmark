#pragma once
#include "Graphics/Vulkan/CommandBuffer/CommandBuffer.hpp"
#include "Graphics/Vulkan/Rendering/RenderPass.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_vulkan.h"
#include <misc/Singleton.hpp>
#include <pch.h>
struct DearImGuiProperties
{
    // std::shared_ptr<GPUInstance> GPU;
};
class DearImGui : public Singleton<DearImGui, DearImGuiProperties>
{
    ImGui_ImplVulkanH_Window IMGUI_WINDOW;

    GPURef<VK::RenderPass> RenderPass;
    GPURef<VK::CommandBuffer> cmdBuffer = GPURef<VK::CommandBuffer>::MakeRef(VK::CmdCapatiblities::eGraphics);

  public:
    DearImGui(const DearImGuiProperties &Properties);
    void BeginFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    void EndFrame()
    {
        ImGuiIO &io = ImGui::GetIO();
        ImGui::Render();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }
    [[nodiscard]] GPURef<VK::CommandBuffer> Render()
    {
        cmdBuffer->Clear();
        //cmdBuffer->Push<VK::BeginRenderPass>(ARGS args...);
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmdBuffer->GetReadyHandle());
        //cmdBuffer->Push<VK::EndRenderPass>(ARGS args...);
		return cmdBuffer;
    }
};