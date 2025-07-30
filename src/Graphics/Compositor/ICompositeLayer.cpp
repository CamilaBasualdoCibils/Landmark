#include "ICompositeLayer.hpp"
#include "CompositeGroup.hpp"
Graphics::ICompositeLayer::ICompositeLayer(std::shared_ptr<CompositeGroup> Parent,
                                           const CompositeLayerProperties &Properties)
    : Properties(Properties)
{
    SetParent(Parent);
    UpdateAttachments();
}

uvec2 Graphics::ICompositeLayer::GetResolution() const
{
    if (Properties.ResolutionOverride.has_value())
    {
        return Properties.ResolutionOverride.value();
    }
    else
    {
        LASSERT(GetParent(), "A Composite Layer without a parent must have a resolution override");
        return GetParent()->GetResolution();
    }
}

void Graphics::ICompositeLayer::UpdateAttachments()
{
    // go for the largest its ever been to prevent further reallocations
    uvec2 NewResolution = glm::max(RealUnderlyingResolution, GetResolution());
    if (glm::any(glm::greaterThan(NewResolution, RealUnderlyingResolution)))
    {
        if (RealUnderlyingResolution != uvec2(0, 0))
            std::cout << "Resizing composite layer attachments.\n";

        RealUnderlyingResolution = NewResolution;
        for (const auto &desiredAttachment : Properties.Attachments)
        {
            Attachments[desiredAttachment.first] = GPURef<Graphics::Texture>::MakeRef(Graphics::TextureProperties{
                .Dimensions = uvec3(NewResolution, 1), .Format = desiredAttachment.second.format});
        }
    }
}

void Graphics::ICompositeLayer::SetResolutionOverride(uvec2 NewResolution)
{

    Properties.ResolutionOverride = NewResolution;
    UpdateAttachments();
}
