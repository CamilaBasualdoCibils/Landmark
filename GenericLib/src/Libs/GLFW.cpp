#include "GLFW.hpp"


void GLFW::GetRequiredVulkanExtensions(std::vector<const char*>& ext)
{
    uint32_t count;
    ext.reserve(ext.size()+count);
    const char** exts = glfwGetRequiredInstanceExtensions(&count);
    for (int i = 0; i < count;i++)
    {
        ext.push_back(exts[i]);
    }
}