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
    TextureFormatValues format = TextureFormatValues::eRGBA8_UNorm;
};
using CompositeLayerAttachments = std::unordered_map<std::string, CompositeLayerAttachmentProperties>;
struct CompositeLayerProperties
{
    std::string Name;
    std::optional<uvec2> ResolutionOverride;
    CompositeLayerAttachments Attachments = {{"Main", CompositeLayerAttachmentProperties{}}};
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
    uvec2 RealUnderlyingResolution = {0, 0};
    virtual CompositeLayerExecute OnRender(const CompositeContext &Context) = 0;

  public:
    ICompositeLayer(std::shared_ptr<CompositeGroup> Parent, const CompositeLayerProperties &Properties);
    virtual ~ICompositeLayer()
    {
    }
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

    void UpdateAttachments();

  public:
    void SetResolutionOverride(uvec2 NewResolution);
    void RemoveResolutionOverride()
    {
        Properties.ResolutionOverride = std::nullopt;
        UpdateAttachments();
    }
    virtual const std::unordered_map<std::string, GPURef<Graphics::Texture>> &GetAttachments()
    {
        return Attachments;
    }
    virtual const std::unordered_map<std::string, GPURef<Graphics::Texture>> &GetAttachments() const
    {
        return Attachments;
    }
    GPURef<Graphics::Texture> GetAttachment(const std::string &Name)
    {
        return GetAttachments().at(Name);
    }
    GPURef<Graphics::Texture> GetAttachment(const std::string &Name) const
    {
        return GetAttachments().at(Name);
    }
};
} // namespace Graphics
