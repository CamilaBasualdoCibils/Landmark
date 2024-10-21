#include <pch.h>
#include <Containers/Flags.h>
#include "PhysicalDevice.h"
#include <GenericEnums.h>
class device_list
{
public:
    
    struct device_value
    {
        device_parameters parameter;
        CompareOp op;
        std::variant<int64_t, double> value;
    };

private:
    using device_container = std::vector<PhysicalDevice>;
    device_container devices;

public:
    device_list(const device_container &);
    const device_container &getDevices() const { return devices; }
    struct filter_mask
    {
        Flags<device_features> features;
        std::vector<device_value> values;
        std::vector<device_parameters> sort_by;
    };
    void Filter(const filter_mask& mask);
};