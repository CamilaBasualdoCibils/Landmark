#include "Pipeline.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "Graphics/Vulkan/Pipeline/ShaderModule.hpp"
void VK::Pipeline::CreateGraphicsPipeline(const PipelineProperties &Properties)
{
    const GraphicsPipelineProperties &GraphicsProperties =
        std::get<GraphicsPipelineProperties>(Properties.VariantProperties);
    vk::PipelineDynamicStateCreateInfo DynamicStatesInfo;
    vk::DynamicState DynamicStates[] = {vk::DynamicState::eViewport};
    DynamicStatesInfo.setDynamicStates(DynamicStates);

    vk::PipelineVertexInputStateCreateInfo VertexInputInfo;
    std::vector<vk::VertexInputAttributeDescription> vk_VertexAttributes;
    std::vector<vk::VertexInputBindingDescription> vk_VertexBindings;
    for (int i = 0; i < GraphicsProperties.VertexAttributes.size();i++)
    {
        const auto& Attrib = GraphicsProperties.VertexAttributes[i];
        vk::VertexInputAttributeDescription vkAttrib;
        vk::VertexInputBindingDescription vkBinding;
        vkAttrib.binding = Attrib.Binding;
        vkAttrib.format = (vk::Format)Attrib.format;
        vkAttrib.location = i;
        vkAttrib.offset = Attrib.Offset;
        vkBinding.binding = Attrib.Binding;
        vkBinding.inputRate = (vk::VertexInputRate)Attrib.Rate;
        vkBinding.stride = Attrib.Stride;
        vk_VertexAttributes.push_back(vkAttrib);
        vk_VertexBindings.push_back(vkBinding);
    }
    VertexInputInfo.setVertexAttributeDescriptions(vk_VertexAttributes);
    VertexInputInfo.setVertexBindingDescriptions(vk_VertexBindings);

    vk::PipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
    InputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;
    vk::PipelineViewportStateCreateInfo ViewportStateInfo;
    vk::Viewport vp = vk::Viewport{0, 0, 512, 512};
    ViewportStateInfo.setViewports({vp});
    vk::Rect2D scissor{{0,0},{512,512}};
    ViewportStateInfo.setScissors({scissor});

    vk::PipelineRasterizationStateCreateInfo RasterizationInfo;
    RasterizationInfo.cullMode = vk::CullModeFlagBits::eNone;
    RasterizationInfo.polygonMode = vk::PolygonMode::eFill;
    RasterizationInfo.frontFace = vk::FrontFace::eClockwise;

    vk::PipelineMultisampleStateCreateInfo MultiSampleInfo;
    MultiSampleInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;

    vk::PipelineColorBlendAttachmentState colorBlendAttachment;
    {
        colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                              vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        colorBlendAttachment.blendEnable = false;
        // TODO: THE REST
    }
    vk::PipelineColorBlendStateCreateInfo ColorBlendInfo;
    ColorBlendInfo.setAttachments({colorBlendAttachment});

    vk::Format format = vk::Format::eR8G8B8A8Unorm;
    vk::PipelineRenderingCreateInfo RenderingInfo;
    RenderingInfo.colorAttachmentCount = 1;
    RenderingInfo.pColorAttachmentFormats = &format;
    RenderingInfo.depthAttachmentFormat = vk::Format::eUndefined;
    RenderingInfo.stencilAttachmentFormat = vk::Format::eUndefined;

    std::vector<vk::PipelineShaderStageCreateInfo> ShaderStageInfos;
    GPURef<VK::ShaderModule> VertexModule = GPURef<VK::ShaderModule>::MakeRef(GraphicsProperties.VertexBinary),
                             FragmentModule = GPURef<VK::ShaderModule>::MakeRef(GraphicsProperties.FragmentBinary),
                             GeometryModule =
                                 GraphicsProperties.GeometryBinary.has_value()
                                     ? GPURef<VK::ShaderModule>::MakeRef(GraphicsProperties.GeometryBinary.value())
                                     : nullptr;

    {
        ShaderStageInfos.emplace_back();
        vk::PipelineShaderStageCreateInfo &VertexStageInfo = ShaderStageInfos.back();
        VertexStageInfo.pName = "main";
        VertexStageInfo.module = *VertexModule;
        VertexStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
    }
    {
        ShaderStageInfos.emplace_back();

        vk::PipelineShaderStageCreateInfo &FragmentStageInfo = ShaderStageInfos.back();
        FragmentStageInfo.pName = "main";
        FragmentStageInfo.module = *FragmentModule;
        FragmentStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
    }
    if (GeometryModule)
    {
        ShaderStageInfos.emplace_back();
        vk::PipelineShaderStageCreateInfo &GeometryStageInfo = ShaderStageInfos.back();
        GeometryStageInfo.pName = "main";
        GeometryStageInfo.module = *GeometryModule;
        GeometryStageInfo.stage = vk::ShaderStageFlagBits::eGeometry;
    }

    vk::GraphicsPipelineCreateInfo CreateInfo;
    CreateInfo.setPVertexInputState(&VertexInputInfo);
    CreateInfo.setPInputAssemblyState(&InputAssemblyInfo);
    CreateInfo.setPViewportState(&ViewportStateInfo);
    CreateInfo.setPRasterizationState(&RasterizationInfo);
    CreateInfo.setPMultisampleState(&MultiSampleInfo);
    CreateInfo.setPColorBlendState(&ColorBlendInfo);
    CreateInfo.setPDynamicState(&DynamicStatesInfo);
    CreateInfo.setLayout(*Properties.Layout);
    CreateInfo.setRenderPass(VK_NULL_HANDLE);
    CreateInfo.setPNext(&RenderingInfo);
    CreateInfo.setStages(ShaderStageInfos);

    const auto CreateResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createGraphicsPipeline(VK_NULL_HANDLE,
                                                                                                      CreateInfo);
    LASSERT(CreateResult.result == vk::Result::eSuccess, "damn");
    Handle = CreateResult.value;
}
void VK::Pipeline::CreateComputePipeline(const PipelineProperties &Properties)
{
}
void VK::Pipeline::CreateRaytracingPipeline(const PipelineProperties &Properties)
{
}
VK::Pipeline::Pipeline(const PipelineProperties &Properties)
{
    LASSERT(Properties.Layout, "No layout provided");
    if (std::holds_alternative<GraphicsPipelineProperties>(Properties.VariantProperties))
    {
        CreateGraphicsPipeline(Properties);
    }
    else if (std::holds_alternative<ComputePipelineProperties>(Properties.VariantProperties))
    {
        CreateComputePipeline(Properties);
    }
    else if (std::holds_alternative<RayTracingPipelineProperties>(Properties.VariantProperties))
    {
        CreateRaytracingPipeline(Properties);
    }
    else
    {
        LASSERT(false, "Invalid use");
    }
}

VK::Pipeline::~Pipeline()
{
    GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().destroyPipeline(Handle);
}
