#pragma once
#include <pch.h>

#include "Payload.hpp"
#include "Layout.hpp"
namespace Render
{
namespace Composite
{
class Pass;
class Pipeline;

class Object : public std::enable_shared_from_this<Object>
{
    friend Pass;
    std::weak_ptr<Pass> ParentPass;

  protected:
    void SetParent(std::weak_ptr<Pass> parent)
    {
        LASSERT(!parent.expired(), "Invalid parent");
        ParentPass = parent;
    }

  public:
    [[nodiscard]] auto GetParent() const
    {
        return ParentPass;
    }
    std::weak_ptr<Pipeline> GetPipeline();
    virtual bool Build() = 0;
    virtual void Compose(Payload &payload) = 0;
    virtual const Layout & GetLayout() const;
};
} // namespace Composite

} // namespace Render
