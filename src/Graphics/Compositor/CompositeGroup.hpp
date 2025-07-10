#pragma once
#include "CompositeContext.hpp"
#include "ICompositeLayer.hpp"
#include <pch.h>
namespace Graphics
{

class CompositeGroup : public ICompositeLayer, public std::enable_shared_from_this<CompositeGroup>
{
    std::vector<std::shared_ptr<ICompositeLayer>> Layers;

  public:
    CompositeGroup(const CompositeLayerProperties &Properties);
    CompositeLayerExecute OnRender(const Graphics::CompositeContext &OldContext) override;
    template <typename T, typename... ARGS> //, std::enable_if_t<std::is_base_of_v<T,ICompositeLayer>>>
    std::shared_ptr<T> AddLayer(ARGS... args)
    {
        Layers.push_back(std::make_shared<T>(shared_from_this(), std::forward<ARGS>(args)...));
        return std::static_pointer_cast<T>(Layers.back());
    }

    GPURef<Graphics::Texture> GetOutput() override
    {
        return Layers.back()->GetOutput();
    }
    const GPURef<Graphics::Texture> GetOutput() const override
    {
        return Layers.back()->GetOutput();
    }
};
} // namespace Graphics
