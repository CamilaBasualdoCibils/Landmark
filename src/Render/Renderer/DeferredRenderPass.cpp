#include "DeferredRenderPass.hpp"

Render::DeferredRenderPass::DeferredRenderPass(std::string_view name)
    : Composite::Pass(name, Composite::PassProperties{.layout = GetDeferredLayout()})
{
}