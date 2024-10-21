#pragma once
#include <pch.h>
#include <VK/Devices/DeviceResource.h>

class DescriptorPool;
class DescriptorSetLayout;

class DescriptorSet final : public DeviceResource
{

    vk::DescriptorSet vk_set;
    vk::DescriptorPool poolOwner;

public:
    DescriptorSet(const DescriptorPool &pool, const DescriptorSetLayout &layout);

    void Destroy() override;
    vk::DescriptorSet GetVkDescriptorSet() const { return vk_set; }
    operator vk::DescriptorSet() const { return vk_set; }
};