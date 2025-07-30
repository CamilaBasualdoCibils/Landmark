
#pragma once
#include <pch.h>
#include <Graphics/Compositor/ICompositeLayer.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/VKCommandManager.hpp>
#include <Graphics/Vulkan/Buffer/VKBuffer.hpp>
#include <Graphics/Vulkan/Pipeline/VKPipeline.hpp>
#include "Render/Components/Entity.hpp"
class CameraRenderer : public Graphics::ICompositeLayer
{
    Entity Owner;
    std::shared_ptr<VK::CommandManager> vkCmdMngr = std::make_shared<VK::CommandManager>();
    GPURef<VK::Pipeline> pipeline;
    GPURef<Graphics::RenderTarget> renderTarget;
    GPURef<VK::Buffer> InstanceVertexBuffer;
    Graphics::CompositeLayerExecute OnRender(const Graphics::CompositeContext& Context) override;
    Graphics::CompositeLayerExecute ExecuteOutput;
    void IssueCommands();
    public:
    CameraRenderer(std::shared_ptr<Graphics::CompositeGroup> Parent,Entity Owner);
    
};