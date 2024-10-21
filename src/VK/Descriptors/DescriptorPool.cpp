#include "DescriptorPool.h"


DescriptorPool::DescriptorPool(const DescriptorPoolProperties &_properties):properties(_properties)
{
        vk::DescriptorPoolSize poolSize{};
    poolSize.descriptorCount = properties._maxSets;

    vk::DescriptorPoolCreateInfo poolInfo{};
    poolInfo.flags = properties.creationFlags;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(properties._maxSets);

    vk_pool = GetvkDevice().createDescriptorPool(poolInfo).value;
    logger.Debug("DescriptorPool Created");
}

void DescriptorPool::Destroy()
{
    GetvkDevice().destroyDescriptorPool(vk_pool);
}
