#pragma once
#include <Graphics/Compositor/ICompositeLayer.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/VKCommandManager.hpp>
#include <Graphics/Vulkan/Rendering/VKRenderTarget.hpp>
#include <misc/Singleton.hpp>
#include <pch.h>
class ImGuiCompositor : public Graphics::ICompositeLayer
{
    std::shared_ptr<VK::CommandManager> GenericBeforeCmdMngr = std::make_shared<VK::CommandManager>(),
                                        ImGuiCmdManager = std::make_shared<VK::CommandManager>(),
                                        GenericAfterCmdMngr = std::make_shared<VK::CommandManager>();

    Graphics::CompositeLayerExecute OnRender(const Graphics::CompositeContext &Context) override;

  public:
    ImGuiCompositor(std::shared_ptr<Graphics::CompositeGroup> Parent);
};
struct DearImGuiProperties
{
    // std::shared_ptr<GPUInstance> GPU;
};
class DearImGui : public Singleton<DearImGui, DearImGuiProperties>
{
    ImGui_ImplVulkanH_Window IMGUI_WINDOW;

    GPURef<VK::CommandManager> cmdManager = GPURef<VK::CommandManager>::MakeRef(VK::CmdCapatiblities::eGraphics);

    std::shared_ptr<ImGuiCompositor> CompositeLayer;

  public:
    DearImGui(const DearImGuiProperties &Properties);
    void BeginFrame();
    void EndFrame();

    void UpdatePlatforms();
    std::shared_ptr<ImGuiCompositor> GetCompositeLayer();

    //[[nodiscard]] GPURef<VK::CommandManager> Render();
};
