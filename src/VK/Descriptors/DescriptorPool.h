#pragma once
#include <pch.h>
#include <GenericEnums.h>
#include <VK/Devices/DeviceResource.h>
class DescriptorPool final : DeviceResource
{
public:
enum class CreationFlags : uint32_t
    {
        /*specifies that descriptor sets allocated from this pool CAN include bindings with the
        UPDATE_AFTER_BIND flag*/
        UPDATE_AFTER_BIND = int_cast(vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind),

        /*allows a descriptor pool to free existing descriptor sets*/
        FREE_DESCRIPTOR_SET = int_cast(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet)
    };
struct DescriptorPoolProperties {
    uint32_t _maxSets = 1;
    Flags<CreationFlags> creationFlags = {CreationFlags::UPDATE_AFTER_BIND,CreationFlags::FREE_DESCRIPTOR_SET};
};
    

private:
    DescriptorPoolProperties properties;
    vk::DescriptorPool vk_pool;

public:
    DescriptorPool(const DescriptorPoolProperties& properties);

    void Destroy() override;

    vk::DescriptorPool GetVkPool() const {return vk_pool;}
    constexpr const DescriptorPoolProperties& GetProperties() const {return properties;}
    operator vk::DescriptorPool() const {return vk_pool;}
};