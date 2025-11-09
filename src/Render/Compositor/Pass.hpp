#pragma once
#include "Layer.hpp"
#include "Layout.hpp"
#include <Graphics/GPURef.hpp>
#include <Graphics/Vulkan/Images/VKTexture.hpp>
#include <pch.h>
namespace Render
{
namespace Composite
{

struct PassProperties
{
    Layout layout;
};
class Pass : public Layer
{
    boost::container::static_vector<LoadedColorAttachment, MAX_COLOR_ATTACHMENTS> ColorAttachments;
    std::optional<LoadedDepthStencilAttachment> DepthStencilAttachment;
    GPURef<VK::RenderTarget> renderTarget;
    Layout layout;
    std::vector<std::shared_ptr<Layer>> Layers;
    // std::optional<RenderPass> renderPass;
    bool Built = false;

    /**
     * @brief Merge this pass's output into the parent pass
     *
     * @param payload
     */
    virtual void ComposeMerge(Payload &payload);

  public:
    Pass(std::string_view Name, const PassProperties &properties);

    void Compose(Payload &Payload) override;
    template <typename T, typename... Args> std::weak_ptr<T> Push(Args &&...args)
    {
        std::shared_ptr<T> newObject = std::make_shared<T>(std::forward<Args>(args)...);
        newObject->SetParent(std::dynamic_pointer_cast<Pass>(shared_from_this()));
        Layers.push_back(newObject);
        return newObject;
    }
    const decltype(Layers) &GetLayers() const
    {
        return Layers;
    }
    const Layout &GetLayout() const override
    {
        return layout;
    }
    bool Build() override;
    bool IsBuilt() const {return Built;}

    const decltype(ColorAttachments)& GetColorImages() const {return ColorAttachments;}
    const decltype(DepthStencilAttachment)& GetDepthStencilImage() const {return DepthStencilAttachment;}
};
} // namespace Composite
} // namespace Render
