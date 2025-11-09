#pragma once
#include <pch.h>
#include <misc/Singleton.hpp>
namespace GL
{
    struct Device
    {
        EGLDeviceEXT Handle = 0;
        std::array<char, 16> DeviceUUID = {0}, DriverUUID = {0};
        std::string DRMPath,RenderNodePath;
    };
    struct Context
    {
        EGLDisplay Display = {0};
        EGLContext Context= {0};
        std::shared_ptr<Device> RenderingDevice;
    };
    class EGL : public Singleton<EGL>
    {
    private:
        std::vector<std::shared_ptr<Device>> AvailableDevices;
        PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT;
        PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT;
        PFNEGLQUERYDEVICESTRINGEXTPROC eglQueryDeviceStringEXT;
        PFNEGLQUERYDEVICEATTRIBEXTPROC eglQueryDeviceAttribEXT;
        PFNEGLQUERYDEVICEBINARYEXTPROC eglQueryDeviceBinaryEXT;
        std::vector<std::shared_ptr<Context>> ActiveContexts;

    public:
        EGL(/* args */);
        [[nodiscard]] std::vector<std::shared_ptr<Device>> EnumerateDevices() const { return AvailableDevices; }
        [[nodiscard]] std::shared_ptr<Context> CreateContext(std::shared_ptr<Device> device);
    };

}
