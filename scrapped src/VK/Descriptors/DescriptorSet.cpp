#include "DescriptorSet.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
DescriptorSet::DescriptorSet(const DescriptorPool &pool, const DescriptorSetLayout &layout):properties_of_owner(pool.GetProperties())
{
    poolOwner = pool;
    vk::DescriptorSetAllocateInfo allocateInfo{};

    allocateInfo.descriptorPool = pool.GetVkPool();
    vk::DescriptorSetLayout _vkLayout = layout.GetVkLayout();
    allocateInfo.pSetLayouts = &_vkLayout;
    allocateInfo.descriptorSetCount = 1;

    const auto allocations = GetvkDevice().allocateDescriptorSets(allocateInfo).value;
    vk_set = allocations.front();



}

void DescriptorSet::Destroy()
{
    LASSERT(properties_of_owner.creationFlags & DescriptorPool::CreationFlags::FREE_DESCRIPTOR_SET,"Descriptor Pool was not created with FREE_DESCRIPTOR_SET flags allowing it to free existing descriptor sets");
    GetvkDevice().freeDescriptorSets(poolOwner,vk_set);
}

void DescriptorSet::UpdateUniformBuffers(int32_t binding, const UniformBuffer &ubo) {
    vk::DescriptorBufferInfo buffer_info;
    buffer_info.buffer = ubo;
    buffer_info.offset = 0;
    buffer_info.range = ubo.Size();

    vk::WriteDescriptorSet descriptorWrite{};
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstSet = vk_set;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &buffer_info;
    GetvkDevice().updateDescriptorSets({descriptorWrite},{});
}
