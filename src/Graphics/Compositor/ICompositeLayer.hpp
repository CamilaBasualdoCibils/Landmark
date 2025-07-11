#pragma once
#include <Graphics/GPURef.hpp>
#include <Graphics/ICommandManager.hpp>
#include <Graphics/Synchronization.hpp>
#include <Graphics/Texture.hpp>
#include <pch.h>
namespace Graphics
{
class CompositeGroup;
struct CompositeContext;
struct CompositeLayerAttachmentProperties
{
    TextureFormatValues format = TextureFormatValues::eRGBA8_SRGB;
};
struct CompositeLayerProperties
{
    std::string Name;
    std::optional<uvec2> ResolutionOverride;
    std::unordered_map<std::string, CompositeLayerAttachmentProperties> Attachments = {
        {"Main", CompositeLayerAttachmentProperties{}}};
};
struct CompositeLayerExecute
{
    std::vector<std::shared_ptr<Graphics::ICommandManager>> CommandManagers;
    // GPURef<Graphics::Semaphore> DoneSemaphore;
};
class ICompositeLayer
{
    std::unordered_map<std::string, GPURef<Graphics::Texture>> Attachments;
    std::shared_ptr<CompositeGroup> Parent = nullptr;
    CompositeLayerProperties Properties;
    virtual CompositeLayerExecute OnRender(const CompositeContext &Context) = 0;

  public:
    ICompositeLayer(std::shared_ptr<CompositeGroup> Parent, const CompositeLayerProperties &Properties);
    CompositeLayerExecute Render(const CompositeContext &Context)
    {
        return OnRender(Context);
    }
    void SetParent(std::shared_ptr<CompositeGroup> NewParent)
    {
        Parent = NewParent;
    }
    std::shared_ptr<CompositeGroup> GetParent() const
    {
        return Parent;
    }
    const CompositeLayerProperties &GetProperties() const
    {
        return Properties;
    }
    uvec2 GetResolution() const;

  public:
    virtual const std::unordered_map<std::string, GPURef<Graphics::Texture>>& GetAttachments()
    {
        return Attachments;
    }
    virtual const std::unordered_map<std::string, GPURef<Graphics::Texture>>& GetAttachments() const
    {
        return Attachments;
    }
};
} // namespace Graphics
