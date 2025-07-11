#pragma once
#include "Graphics/Interop/Interop.hpp"
#include "Graphics/Vulkan/Enums.hpp"
#include <pch.h>

namespace VK
{
struct SamplerProperties
{
    SamplerAddressMode AddressModesU = SamplerAddressMode::eRepeat, AddressModesV = SamplerAddressMode::eRepeat,
                       AddressModesW = SamplerAddressMode::eRepeat;
    bool AnisotropyEnable = false;
    float maxAnisotropy = 1;
    Filter magFilter = Filter::eNearest;
    Filter minFilter = Filter::eNearest;
    float MaxLod = 1;
    float MinLod = 0;
    float mipLodBias = 0.5;
    SamplerMipMapMode mipmapMode = SamplerMipMapMode::eLinear;
};
class Sampler
{
    vk::Sampler Handle;

  public:
    Sampler(const SamplerProperties &Properties);
    vk::Sampler GetHandle() const
    {
        return Handle;
    }
    operator vk::Sampler() const
    {
        return GetHandle();
    }
};
} // namespace VK