#pragma once

#include <pch.h>

namespace Graphics
{
struct RenderTargetProperties;
}
namespace GL
{
class RenderTarget
{
    public:
    RenderTarget(const Graphics::RenderTargetProperties& Properties);
};
} // namespace GL