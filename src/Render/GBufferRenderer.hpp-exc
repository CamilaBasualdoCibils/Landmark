
#pragma once
#include "Render/Components/Entity.hpp"
#include <Graphics/Compositor/ICompositeLayer.hpp>
#include <Graphics/Vulkan/Buffer/VKBuffer.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/VKCommandManager.hpp>
#include <Graphics/Vulkan/Pipeline/VKPipeline.hpp>
#include <pch.h>

class GBufferRenderer : public Graphics::ICompositeLayer
{
    Entity Owner;
    std::shared_ptr<VK::CommandManager> vkCmdMngr = std::make_shared<VK::CommandManager>();
    GPURef<VK::Pipeline> pipeline;
    GPURef<VK::RenderTarget> renderTarget;
    GPURef<VK::Buffer> InstanceVertexBuffer;
    Graphics::CompositeLayerExecute OnRender(const Graphics::CompositeContext &Context) override;
    Graphics::CompositeLayerExecute ExecuteOutput;


  public:
    GBufferRenderer(std::shared_ptr<Graphics::CompositeGroup> Parent, Entity Owner);
};