#pragma once
#include <pch.h>
#include <Graphics/GPURef.hpp>
#include <Graphics/Vulkan/Pipeline/VKPipelineLayout.hpp>
#include "Graphics/Vulkan/VKEnums.hpp"
namespace VK
{


struct VertexAttribute
{
    uint32_t Binding; //which vertex buffer binding to take from
    Format format; 
    uint32_t Stride;
    uint32_t Offset;
    VertexInputRate Rate = VertexInputRate::eVertex;
};
struct GraphicsPipelineProperties
{
    std::vector<std::byte> VertexBinary, FragmentBinary;
    std::optional<std::vector<std::byte>> GeometryBinary;
    std::vector<VertexAttribute> VertexAttributes;

        VK::CullMode cullMode = VK::CullMode::eBack;
    //Depth Testing
        bool DepthTest = false,DepthWrite = false;
        VK::CompareOp DepthComparison = VK::CompareOp::eLess;
        VK::Format DepthAttachmentFormat = VK::Format::eDepth32_SFloat;
        bool RasterizerDiscard = false; // Makes sure that geometry never proceeds to the raster stage
        bool StencilTest = false;
};
struct ComputePipelineProperties
{
    std::vector<std::byte> ComputeBinary;
};
struct RayTracingPipelineProperties
{
};
struct PipelineProperties
{
    GPURef<VK::PipelineLayout> Layout;
    std::variant<GraphicsPipelineProperties, ComputePipelineProperties, RayTracingPipelineProperties> VariantProperties;
};
class Pipeline
{
    vk::Pipeline Handle;

    void CreateGraphicsPipeline(const PipelineProperties &Properties);
    void CreateComputePipeline(const PipelineProperties &Properties);
    void CreateRaytracingPipeline(const PipelineProperties &Properties);
  public:
    Pipeline(const PipelineProperties &Properties);
    ~Pipeline();
    operator vk::Pipeline() const
    {
        return Handle;
    }
};
} // namespace VK