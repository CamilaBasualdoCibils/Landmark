#pragma once
#include <pch.h>
#include <Graphics/Compositor/ICompositeLayer.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/CommandManager.hpp>


class ImGuiCompositor : public Graphics::ICompositeLayer
{
    std::shared_ptr<VK::CommandManager> GenericBeforeCmdMngr = std::make_shared<VK::CommandManager>(),ImGuiCmdManager = std::make_shared<VK::CommandManager>(),GenericAfterCmdMngr = std::make_shared<VK::CommandManager>();

    Graphics::CompositeLayerExecute OnRender(const Graphics::CompositeContext& Context) override;
    public:
    ImGuiCompositor(std::shared_ptr<Graphics::CompositeGroup> Parent);
    
};