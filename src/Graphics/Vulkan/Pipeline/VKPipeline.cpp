#include "VKPipeline.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "Graphics/Vulkan/Pipeline/VKShaderModule.hpp"
void VK::Pipeline::CreateGraphicsPipeline(const PipelineProperties &Properties)
{
    const GraphicsPipelineProperties &GraphicsProperties =
        std::get<GraphicsPipelineProperties>(Properties.VariantProperties);
    vk::PipelineDynamicStateCreateInfo DynamicStatesInfo;
    vk::DynamicState DynamicStates[] = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
    DynamicStatesInfo.setDynamicStates(DynamicStates);

    vk::PipelineVertexInputStateCreateInfo VertexInputInfo;
    std::vector<vk::VertexInputAttributeDescription> vk_VertexAttributes;
    std::vector<vk::VertexInputBindingDescription> vk_VertexBindings;
    std::unordered_map<uint32_t, vk::VertexInputBindingDescription> bindingMap;
    for (int i = 0; i < GraphicsProperties.VertexAttributes.size(); i++)
    {
        const auto &Attrib = GraphicsProperties.VertexAttributes[i];
        vk::VertexInputAttributeDescription vkAttrib;

        vkAttrib.binding = Attrib.Binding;
        vkAttrib.format = (vk::Format)Attrib.format;
        vkAttrib.location = i;
        vkAttrib.offset = Attrib.Offset;

        vk_VertexAttributes.push_back(vkAttrib);
        if (!bindingMap.contains(Attrib.Binding))
        {
            vk::VertexInputBindingDescription vkBinding;
            vkBinding.binding = Attrib.Binding;
            vkBinding.inputRate = (vk::VertexInputRate)Attrib.Rate;
            vkBinding.stride = Attrib.Stride;
            bindingMap[Attrib.Binding] = vkBinding;
        }
    }
    for (const auto &kv : bindingMap)
    {
        vk_VertexBindings.push_back(kv.second);
    }
    VertexInputInfo.setVertexAttributeDescriptions(vk_VertexAttributes);
    VertexInputInfo.setVertexBindingDescriptions(vk_VertexBindings);

    vk::PipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
    InputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;
    vk::PipelineViewportStateCreateInfo ViewportStateInfo;
    vk::Viewport vp = vk::Viewport{0, 0, 512, 512};
    ViewportStateInfo.setViewports({vp});
    vk::Rect2D scissor{{0, 0}, {512, 512}};
    ViewportStateInfo.setScissors({scissor});

    vk::PipelineRasterizationStateCreateInfo RasterizationInfo;
    RasterizationInfo.cullMode = (vk::CullModeFlagBits)GraphicsProperties.cullMode;
    RasterizationInfo.polygonMode = vk::PolygonMode::eFill;
    RasterizationInfo.frontFace = vk::FrontFace::eCounterClockwise;
    RasterizationInfo.rasterizerDiscardEnable = GraphicsProperties.RasterizerDiscard;
    RasterizationInfo.lineWidth = 1.0f;

    vk::PipelineMultisampleStateCreateInfo MultiSampleInfo;
    MultiSampleInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;

    std::vector<vk::PipelineColorBlendAttachmentState> ColorBlendingAttachments;
    ColorBlendingAttachments.resize(GraphicsProperties.ColorAttachments.size());
    for (int i = 0; i < GraphicsProperties.ColorAttachments.size(); i++)
    {
        const auto &blendProp = GraphicsProperties.ColorAttachments[i].Blending;

        vk::PipelineColorBlendAttachmentState &colorBlendAttachment = ColorBlendingAttachments[i];
        colorBlendAttachment.colorWriteMask = blendProp.ColorComponents;
        colorBlendAttachment.blendEnable = blendProp.BlendEnable;
        colorBlendAttachment.srcColorBlendFactor = (vk::BlendFactor)blendProp.SrcColorFactor;
        colorBlendAttachment.dstColorBlendFactor = (vk::BlendFactor)blendProp.DstColorFactor;
        colorBlendAttachment.colorBlendOp = (vk::BlendOp)blendProp.ColorBlendOp;

        colorBlendAttachment.srcAlphaBlendFactor = (vk::BlendFactor)blendProp.SrcAlphaFactor;
        colorBlendAttachment.dstAlphaBlendFactor = (vk::BlendFactor)blendProp.DstAlphaFactor;
        colorBlendAttachment.alphaBlendOp = (vk::BlendOp)blendProp.AlphaBlendOp;
    }

    vk::PipelineColorBlendStateCreateInfo ColorBlendInfo;
    ColorBlendInfo.setAttachments(ColorBlendingAttachments);

    vk::PipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.depthTestEnable = GraphicsProperties.DepthTest;
    depthStencil.depthWriteEnable = GraphicsProperties.DepthWrite;
    depthStencil.depthCompareOp = (vk::CompareOp)GraphicsProperties.DepthComparison;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = GraphicsProperties.StencilTest;

    std::vector<vk::Format> ColorFormats;
    for (const auto &colorAttachment : GraphicsProperties.ColorAttachments)
    {
        ColorFormats.push_back((vk::Format)colorAttachment.format);
    }
    vk::Format DepthFormat = (vk::Format)GraphicsProperties.DepthAttachment.format,
               StencilFormat = vk::Format::eUndefined;
    vk::PipelineRenderingCreateInfo RenderingInfo;
    RenderingInfo.setColorAttachmentFormats(ColorFormats);
    RenderingInfo.depthAttachmentFormat = DepthFormat;
    RenderingInfo.stencilAttachmentFormat = StencilFormat;

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
    CreateInfo.setPDepthStencilState(&depthStencil);
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
