#include <pch.h>
#include <GenericEnums.h>

using BindlessID = uint32_t;
class BindlessResourceHandle
{

public:
    DescriptorTypes type;
    BindlessID ID = -1;
    BindlessResourceHandle(DescriptorTypes t, BindlessID _ID) : type(t), ID(_ID)
    {
    }
    BindlessResourceHandle(const BindlessResourceHandle& o) = default;
    BindlessResourceHandle& operator=(const BindlessResourceHandle& o) = default;
};

class BLImageHandle : public BindlessResourceHandle
{

public:
    BLImageHandle(BindlessID _ID) : BindlessResourceHandle(DescriptorTypes::COMBINED_IMAGE_SAMPLER, _ID) {

                                    };

    BLImageHandle(const BindlessResourceHandle &o) : BindlessResourceHandle(o) {};
    BLImageHandle(const BLImageHandle& o) =default;
    BLImageHandle& operator=(const BLImageHandle& o) = default;
};