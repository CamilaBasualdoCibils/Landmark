#include "ICompositeLayer.hpp"
#include "CompositeGroup.hpp"
Graphics::ICompositeLayer::ICompositeLayer(std::shared_ptr<CompositeGroup> Parent,const CompositeLayerProperties &Properties) : Properties(Properties)
{
    SetParent(Parent);
    Texture = GPURef<Graphics::Texture>::MakeRef(Graphics::TextureProperties{
        .Dimensions = uvec3(GetResolution(), 1),
        .Format = Graphics::TextureFormatValues::eRGBA8_SRGB
    });
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
