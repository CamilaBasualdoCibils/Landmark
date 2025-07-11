#pragma once
#include <pch.h>
#include <Graphics/Compositor/ICompositeLayer.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/CommandManager.hpp>
#include <Graphics/Vulkan/Buffer/Buffer.hpp>
#include <Graphics/Vulkan/Pipeline/Pipeline.hpp>
class WorldCompositor : public Graphics::ICompositeLayer
{
   std::shared_ptr<VK::CommandManager> vkCmdMngr = std::make_shared<VK::CommandManager>();
       GPURef<VK::Pipeline> pipeline;
       GPURef<Graphics::RenderTarget> renderTarget;
       GPURef<VK::Buffer> InstanceVertexBuffer;
    Graphics::CompositeLayerExecute OnRender(const Graphics::CompositeContext& Context) override;
    public:
    WorldCompositor(std::shared_ptr<Graphics::CompositeGroup> Parent);
    
};