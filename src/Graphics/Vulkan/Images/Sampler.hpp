#pragma once
#include "Graphics/Interop/Interop.hpp"
#include <pch.h>
#include <vulkan/vulkan_handles.hpp>

namespace VK
{
struct SamplerProperties
{
};
class Sampler
{
    vk::Sampler Handle;
    public:
    Sampler(const SamplerProperties& Properties){}
    vk::Sampler GetHandle() const {return Handle;}
    operator vk::Sampler() const {return GetHandle();}

};
} // namespace VK