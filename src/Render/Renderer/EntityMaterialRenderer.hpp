#include "Render/Compositor/Layer.hpp"
#include "pch.h"
#include <Graphics/Vulkan/Buffer/VKBuffer.hpp>
#include <Graphics/Vulkan/Pipeline/VKPipeline.hpp>
namespace Render
{
class EntityMaterialRenderer : public Composite::Layer
{
    GPURef<VK::Pipeline> pipeline;
    GPURef<VK::Buffer> InstanceVertexBuffer;
    bool Build() override;
    void Compose(Composite::Payload &payload) override;
};
} // namespace Render
