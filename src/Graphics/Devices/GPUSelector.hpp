#include <pch.h>
#include <Graphics/Devices/GPUDevice.hpp>
#include <misc/Singleton.hpp>
struct eGPUProperties
{
    enum Type
    {
        eVramAmount,
        eVulkanVersion,
        eOpenGLVersion,
    };
};
struct GPUSelectorFilter
{
};
class GPUSelector : public Singleton<GPUSelector>
{
    private:
     std::vector<std::shared_ptr<GPUDevice>> AvailableDevices;
public:
    GPUSelector();
    [[nodiscard]] std::vector<std::shared_ptr<GPUDevice>> FilterDevices(const GPUSelectorFilter &filter);
    [[nodiscard]] const std::vector<std::shared_ptr<GPUDevice>>& AllDevices();
};
