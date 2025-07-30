#include <pch.h>
#include <Graphics/OpenGL/GLBuffer.hpp>
#include <Graphics/Vulkan/VKBuffer.hpp>
#include <Graphics/Vulkan/VKDeviceMemory.hpp>
class Buffer
{
private:
    GL::Buffer GLBuffer;
    VK::Buffer VKBuffer;
    VK::DeviceMemory memory;

public:
    Buffer(/* args */);
    ~Buffer();
};

Buffer::Buffer(/* args */)
{
}

Buffer::~Buffer()
{
}
