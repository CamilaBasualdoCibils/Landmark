#pragma once
#include <pch.h>
#include <VK/Buffers/UniformBuffer.h>
#include <VK/Devices/DeviceResource.h>

#include <VK/Descriptors/DescriptorPool.h>
class DescriptorSetLayout;

class DescriptorSet final : public DeviceResource
{

    vk::DescriptorSet vk_set;
    vk::DescriptorPool poolOwner;
    const DescriptorPool::DescriptorPoolProperties properties_of_owner;

public:
    DescriptorSet(const DescriptorPool &pool, const DescriptorSetLayout &layout);

    void Destroy() override;
    vk::DescriptorSet GetVkDescriptorSet() const { return vk_set; }
    operator vk::DescriptorSet() const { return vk_set; }


    void UpdateUniformBuffers(int32_t binding, const UniformBuffer& ubo);
};