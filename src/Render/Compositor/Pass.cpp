#include "Pass.hpp"
#include "Pipeline.hpp"
void Render::Composite::Pass::Compose(Payload &payload)
{
    Payload subPayload;
    subPayload.pipelinePayload = payload.pipelinePayload;
    subPayload.RenderTarget = renderTarget;
    subPayload.ColorAttachments = ColorAttachments;
    subPayload.DepthAttachment = DepthStencilAttachment;

    // Calls sub passes to compose their subpasses
    for (const auto Layer : GetLayers())
    {
        if (auto subpass = std::dynamic_pointer_cast<Pass>(Layer); subpass)
        {
            subpass->Compose(subPayload);
        }
    }
    payload.pipelinePayload.cmdManager->PushGroupLabel(GetName());
    payload.pipelinePayload.cmdManager->BeginRendering(renderTarget);
    // runs through THIS layers but if pass the compose merge
    for (const auto Layer : GetLayers())
    {
        if (auto subpass = std::dynamic_pointer_cast<Pass>(Layer); subpass)
        {
            subpass->ComposeMerge(subPayload);
        }
        else
        {

            Layer->Compose(subPayload);
        }
    }
    payload.pipelinePayload.cmdManager->EndRendering();
    payload.pipelinePayload.cmdManager->PopGroupLabel();
}

void Render::Composite::Pass::ComposeMerge(Payload &payload)
{


    
}

Render::Composite::Pass::Pass(std::string_view Name, const PassProperties &properties) : Layer(Name)
{
    layout = properties.layout;
}
bool Render::Composite::Pass::Build()
{
    bool BuildStatus = true;

    ColorAttachments.clear();
    DepthStencilAttachment.reset();
    const ivec2 BaseResolution = GetPipeline().lock()->GetEntrySettings().BaseResolution;
    ColorAttachments.reserve(layout.GetColorAttachments().size());
    renderTarget = GPURef<VK::RenderTarget>::MakeRef();
    renderTarget->SetViewport({0, 0}, BaseResolution);
    uint32 ColorIndex = 0;
    for (const auto &colorAtt : layout.GetColorAttachments())
    {
        LoadedColorAttachment LColorAtt;
        LColorAtt.info = colorAtt;
        VK::TextureProperties TextProps;
        TextProps.ImageProp.Dimensions = ivec3(colorAtt.DimensionsOverride.value_or(BaseResolution), 1);

        TextProps.ImageProp.format = colorAtt.format;
        TextProps.ImageProp.Layers = 1;
        TextProps.ImageProp.memoryProperties = VK::MemoryProperties::eDeviceLocal;
        TextProps.ImageProp.MipLevels = 1;
        TextProps.ImageProp.Samples = 1;
        TextProps.ImageProp.Usage = {VK::ImageUsage::eColorAttachment, VK::ImageUsage::eSampled,
                                     VK::ImageUsage::eTransferDst, VK::ImageUsage::eTransferSrc};
        TextProps.ImageViewProp.AspectMask = {VK::ImageAspect::eColor};

        LColorAtt.Texture = GPURef<VK::Texture>::MakeRef(TextProps);
        ColorAttachments.push_back(LColorAtt);
        renderTarget->AttachColor(ColorIndex, LColorAtt.Texture);
        ColorIndex++;
    }
    if (auto depthAttachment = layout.GetDepthStencilAttachment(); depthAttachment.has_value())
    {
        LoadedDepthStencilAttachment LDepthAtt;
        VK::TextureProperties TextProps;
        TextProps.ImageProp.Dimensions =
            ivec3(depthAttachment->DimensionsOverride.value_or(GetPipeline().lock()->GetEntrySettings().BaseResolution), 1);
        TextProps.ImageProp.format =depthAttachment->format;
        TextProps.ImageProp.Layers = 1;
        TextProps.ImageProp.memoryProperties = VK::MemoryProperties::eDeviceLocal;
        TextProps.ImageProp.MipLevels = 1;
        TextProps.ImageProp.Samples = 1;
        TextProps.ImageProp.Usage = {VK::ImageUsage::eDepthStencilAttachment, VK::ImageUsage::eSampled,
                                     VK::ImageUsage::eTransferDst, VK::ImageUsage::eTransferSrc};
        TextProps.ImageViewProp.AspectMask = {VK::ImageAspect::eDepth};
        LDepthAtt.Texture = GPURef<VK::Texture>::MakeRef(TextProps);
        DepthStencilAttachment = LDepthAtt;
        renderTarget->AttachDepth(LDepthAtt.Texture);
    }

    for (std::shared_ptr<Object> obj : GetLayers())
    {
        BuildStatus = BuildStatus && obj->Build();
        if (!BuildStatus)
            break;
    }
    Built = BuildStatus;
    return BuildStatus;
}