#pragma once
#include "Graphics/Compositor/ICompositeLayer.hpp"
#include "Graphics/Rendering/RenderTarget.hpp"
#include "Graphics/Vulkan/Commands/CommandManager/VKCommandManager.hpp"
#include "Graphics/Vulkan/Pipeline/VKPipeline.hpp"
#include "Render/Components/Entity.hpp"
#include <pch.h>
class EditorViewRenderer : public Graphics::ICompositeLayer
{
    Graphics::CompositeLayerExecute OnRender(const Graphics::CompositeContext &Context) override;
    GPURef<VK::RenderTarget> RenderTarget;
    GPURef<VK::PipelineLayout> GridPipelineLayout;
    GPURef<VK::Pipeline> GridPipeline;
    Entity CameraEntity;
    struct GridPushConstants
    {
        mat4 InvViewProjMat;
        mat4 ViewProjMat;
        vec3 CameraPosition;
    };
    GPURef<VK::CommandManager> CmdManager;

  public:
    EditorViewRenderer(std::shared_ptr<Graphics::CompositeGroup> Parent, Entity CameraEntity);
};