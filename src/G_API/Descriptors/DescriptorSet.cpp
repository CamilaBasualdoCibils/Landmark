#include "DescriptorSet.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
DescriptorSet::DescriptorSet(const DescriptorPool &pool, const DescriptorSetLayout &layout)
{
    poolOwner = pool;
    vk::DescriptorSetAllocateInfo allocateInfo{};

    allocateInfo.descriptorPool = pool.GetVkPool();
    vk::DescriptorSetLayout _vkLayout = layout.GetVkLayout();
    allocateInfo.pSetLayouts = &_vkLayout;
    allocateInfo.descriptorSetCount = 1;
    vk_set = GetvkDevice().allocateDescriptorSets(allocateInfo).value.front();

    logger.Debug("DescriptorSet Created");

}

void DescriptorSet::Destroy()
{
    GetvkDevice().freeDescriptorSets(poolOwner,vk_set);
}
