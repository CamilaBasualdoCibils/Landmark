#include "DescriptorSetLayout.h"

DescriptorSetLayout::DescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding> &_bindingLayout, const Flags<CreationFlags> &createFlags)
    : bindingLayout(_bindingLayout), flags(createFlags)
{

    std::vector<vk::DescriptorSetLayoutBinding> bindings(_bindingLayout.size());
    std::vector<vk::DescriptorBindingFlags> bindingFlags(_bindingLayout.size());

    for (int i = 0; i < _bindingLayout.size(); i++)
    {
        auto &vkbinding = bindings[i];
        auto &vkflags = bindingFlags[i];
        const auto &binding = _bindingLayout[i];

        vkbinding.binding = i;
        vkbinding.descriptorType = enum_cast(binding.type);
        vkbinding.stageFlags = binding.stageFlags;
        vkflags = binding.flags;
        vkbinding.descriptorCount = binding.descriptorCount;
    }

    vk::DescriptorSetLayoutBindingFlagsCreateInfo vkbindingFlags{};
    vkbindingFlags.pNext = nullptr;
    vkbindingFlags.pBindingFlags = bindingFlags.data();
    vkbindingFlags.bindingCount = _bindingLayout.size();

    vk::DescriptorSetLayoutCreateInfo createInfo{};
    createInfo.bindingCount = bindings.size();
    createInfo.pBindings = bindings.data();
    createInfo.flags = flags;
    createInfo.pNext = &vkbindingFlags;

    vk_layout = GetvkDevice().createDescriptorSetLayout(createInfo).value;

}