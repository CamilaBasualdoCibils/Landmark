#include <pch.h>
#include <G_API/Descriptors/DescriptorPool.h>
#include <G_API/Descriptors/DescriptorSet.h>
#include <G_API/Descriptors/DescriptorSetLayout.h>
#include <G_API/Devices/DeviceResource.h>
#include <G_API/GenericEnums.h>
#include <optional>
#include <Containers/ObjectAllocator-new.h>
#include "BindlessResourceHandle.h"
#include <G_API/Images/CombinedImageSampler.h>
#include <variant>
#include <G_API/Descriptors/DSetManager.h>
class BindlessDescriptorSet final : DeviceResource
{

    using SetBinding = uint32_t;
    std::map<DescriptorTypes, std::pair<SetBinding, uint32_t>> bindingAndCountPerType;
    std::optional<DescriptorSetLayout> setLayout;
    std::optional<DSetManager> setManager;

    using IDTrackerType = ObjectAllocator_new<int>;
    std::map<DescriptorTypes, IDTrackerType> IDTrackers;

    IDTrackerType &GetTracker(DescriptorTypes);
    bool CheckIfFull(DescriptorTypes t)
    {
        return GetTracker(t).Count() >= bindingAndCountPerType[t].second;
    }
    BindlessResourceHandle PushResource(DescriptorTypes t);
    void PopResource(const BindlessResourceHandle&);
    bool ContainsResource(const BindlessResourceHandle&);
    
    /// @brief 
    /// @param handle 
    /// @param image If set to nullptr, removes entry from descriptor set at handle
    void WriteCombinedImageSampler(const BLImageHandle& handle, std::variant<std::nullptr_t,const CombinedImageSampler *> image);
public:
    BindlessDescriptorSet(const std::vector<std::pair<DescriptorTypes, uint32_t>> &_maxPerType, const Flags<ShaderStage> &_stages = ShaderStage::ALL);
    void Destroy() override;

    BLImageHandle PushCombinedImageSampler(const CombinedImageSampler& image );
    void ReleaseCombinedImageSampler(const BLImageHandle& handle);
};