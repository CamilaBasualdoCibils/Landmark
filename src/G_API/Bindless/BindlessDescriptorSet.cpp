#include "BindlessDescriptorSet.h"
#include <G_API/Devices/LogicalDevice.h>
BindlessDescriptorSet::IDTrackerType &BindlessDescriptorSet::GetTracker(DescriptorTypes t)
{
    if (!bindingAndCountPerType.contains(t))
        logger.Error("this BindlessDescriptorSet does not support descriptors of type: " + std::to_string(static_cast<std::underlying_type<decltype(t)>::type>(t)));

    return IDTrackers[t];
}

BindlessResourceHandle BindlessDescriptorSet::PushResource(DescriptorTypes t)

{
    IDTrackerType &tracker = GetTracker(t);
    if (CheckIfFull(t))
    {
        logger.Error("BindlessDescriptorSet is at capacity for the descriptor type: " + std::to_string(static_cast<std::underlying_type<decltype(t)>::type>(t)));
        LASSERT(false, "Idiot");
    }

    auto ID = tracker.Push(1).GetIndex();

    return BindlessResourceHandle(t, ID);
}

void BindlessDescriptorSet::PopResource(const BindlessResourceHandle &h)
{
    GetTracker(h.type).Erase(h.ID);
}

bool BindlessDescriptorSet::ContainsResource(const BindlessResourceHandle &h)
{
    return GetTracker(h.type)
    .Contains(h.ID);
}

void BindlessDescriptorSet::WriteCombinedImageSampler(const BLImageHandle &handle, std::variant<std::nullptr_t, const CombinedImageSampler *> image)
{
    vk::DescriptorImageInfo image_info;
    image_info.imageLayout = EnumCast(ImageLayouts::SHADER_READ_ONLY);
    if (std::holds_alternative<std::nullptr_t>(image))
    {
        image_info.imageView = nullptr;
        image_info.sampler = nullptr;
    }
    else
    {
        const CombinedImageSampler &im = *std::get<const CombinedImageSampler *>(image);
        image_info.imageView = im.GetImageView();
        image_info.sampler = im.GetSampler();
    }

    vk::WriteDescriptorSet write{};
    write.descriptorType = EnumCast(DescriptorTypes::COMBINED_IMAGE_SAMPLER);
    write.dstBinding = bindingAndCountPerType[DescriptorTypes::COMBINED_IMAGE_SAMPLER].first;
    write.dstSet = setManager.value().front();
    // Write one texture that is being added
    write.descriptorCount = 1;
    // The array element that we are going to write to
    // is the index, which we refer to as our handles
    write.dstArrayElement = handle.ID;
    write.pImageInfo = &image_info;

    GetDevice()->updateDescriptorSets(1, &write, 0, nullptr);
}


BindlessDescriptorSet::BindlessDescriptorSet(const std::vector<std::pair<DescriptorTypes, uint32_t>> &_maxPerType, const Flags<ShaderStage> &_stages)
{
    std::vector<DescriptorSetLayoutBinding> setLayoutBindings(_maxPerType.size());

    for (int i = 0; i < _maxPerType.size(); i++)
    {
        const auto &typeNcount = _maxPerType[i];

        if (bindingAndCountPerType.contains(typeNcount.first))

        {
            logger.Error("This Bindless Descriptor Set already has an entry for " + vk::to_string(static_cast<vk::DescriptorType>(typeNcount.first)));
            return;
        }
        bindingAndCountPerType.emplace(std::make_pair(typeNcount.first, std::make_pair(i, typeNcount.second)));
        IDTrackers.emplace(typeNcount.first, IDTrackerType());
        setLayoutBindings[i].type = typeNcount.first;
        setLayoutBindings[i].descriptorCount = typeNcount.second;
        setLayoutBindings[i].stageFlags = _stages;
    }

    DSetManager::DSetManagerProrperties dsetProp;
    dsetProp.poolProperties._maxSets = 1;
    dsetProp. poolProperties.creationFlags = DescriptorPool::CreationFlags::UPDATE_AFTER_BIND;
    setLayout.emplace(setLayoutBindings, DescriptorSetLayout::CreationFlags::UPDATE_AFTER_BIND_POOL);
    setManager.emplace(dsetProp);
    setManager->emplace_back(setLayout.value());

    logger.Debug("Bindless DescriptorSet Created");

}

void BindlessDescriptorSet::Destroy()
{
    
    setLayout->Destroy();
    setManager->Destroy();
    logger.Debug("Bindless Descriptor Set Destroyed");
}

BLImageHandle BindlessDescriptorSet::PushCombinedImageSampler(const CombinedImageSampler &image)
{
    auto handle = PushResource(DescriptorTypes::COMBINED_IMAGE_SAMPLER);

    BLImageHandle imageHandle(handle);
    WriteCombinedImageSampler(imageHandle, &image);

    return imageHandle;
}

void BindlessDescriptorSet::ReleaseCombinedImageSampler(const BLImageHandle &handle)
{
    LASSERT(ContainsResource(handle), "Resource of this type and ID does not exist");
    PopResource(handle);
    //DELETING FROM A DESCRIPTORSET REQUIRES AN EXTENSION
    //WriteCombinedImageSampler(handle, nullptr);
}
