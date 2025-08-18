#include "VKSampler.hpp"
#include "Graphics/GraphicsEngine.hpp"

VK::Sampler::Sampler(const VK::SamplerProperties &Properties)
{
    vk::SamplerCreateInfo CreateInfo;
    CreateInfo.addressModeU = static_cast<vk::SamplerAddressMode>(Properties.AddressModesU);
    CreateInfo.addressModeV = static_cast<vk::SamplerAddressMode>(Properties.AddressModesV);
    CreateInfo.addressModeW = static_cast<vk::SamplerAddressMode>(Properties.AddressModesW);
    CreateInfo.anisotropyEnable = Properties.AnisotropyEnable;
    CreateInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
    CreateInfo.compareEnable = false;
    CreateInfo.magFilter = static_cast<vk::Filter>(Properties.magFilter);
    CreateInfo.minFilter = static_cast<vk::Filter>(Properties.minFilter);
    CreateInfo.maxAnisotropy = Properties.maxAnisotropy;
    CreateInfo.maxLod = Properties.MaxLod;
    CreateInfo.minLod = Properties.MinLod;
    CreateInfo.mipLodBias = Properties.mipLodBias;
    CreateInfo.mipmapMode = static_cast<vk::SamplerMipmapMode>(Properties.mipmapMode);
    const auto CreateResult =
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createSampler(CreateInfo);
    Handle = CreateResult.value;
    LASSERT(CreateResult.result == vk::Result::eSuccess, "Failure");
}

VK::Sampler::~Sampler()
{
    GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().destroy(Handle);
}
