#include <pch.h>
#include <Graphics/OpenGL/Buffer.hpp>
#include <Graphics/Vulkan/Buffer.hpp>
#include <Graphics/Vulkan/DeviceMemory.hpp>
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
