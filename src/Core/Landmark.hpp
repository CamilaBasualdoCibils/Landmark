#include <pch.h>
#include <misc/Singleton.hpp>
#include <Graphics/Devices/GPUInstance.hpp>
struct LandmarkStartProperties
{
    
};
class Landmark : public Singleton<Landmark,LandmarkStartProperties>
{
    private:

public:
    Landmark(const LandmarkStartProperties& Properties);
};