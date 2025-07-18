#pragma once
#include <pch.h>
#include <VK/Devices/DeviceResource.h>
#include <Types/Flags.hpp>
#include <GenericEnums.h>
struct DescriptorSetLayoutBinding
{
    DescriptorTypes type;
    uint32_t descriptorCount;
    Flags<ShaderStage> stageFlags = ShaderStage::ALL; // stages in which this layout binding can be used
    Flags<DescriptorBindingFlags> flags = {DescriptorBindingFlags::PARTIALLY_BOUND,
                                           DescriptorBindingFlags::UPDATE_AFTER_BIND};
};
class DescriptorSetLayout final : DeviceResource
{
public:
    enum class CreationFlags : uint32_t
    {
        /*specifies that descriptor sets using this layout must be allocated from a descriptor pool
        created with the UPDATE_AFTER_BIND flag*/
        UPDATE_AFTER_BIND_POOL = int_cast(vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool)
    };

private:
     std::vector<DescriptorSetLayoutBinding> bindingLayout;
     Flags<CreationFlags> flags;
    vk::DescriptorSetLayout vk_layout;

public:
    DescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding> &bindings, const Flags<CreationFlags> &createFlags = CreationFlags::UPDATE_AFTER_BIND_POOL);

    void Destroy() override { GetvkDevice().destroyDescriptorSetLayout(vk_layout); }
    operator vk::DescriptorSetLayout() const { return vk_layout; }
    vk::DescriptorSetLayout GetVkLayout() { return vk_layout; }
    vk::DescriptorSetLayout GetVkLayout() const { return vk_layout; }
};