#include "GLFW.hpp"
#include <GLFW/glfw3.h>

GLFW::GLFW()
{
    if (glfwInit() != GLFW_TRUE)
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwSetErrorCallback(ErrorCallback);
}
void GLFW::ErrorCallback(int code, const char *desc)
{
}
std::vector<VK::Extension> GLFW::GetRequiredVKExtensions() const
{
    std::vector<VK::Extension> ExtensionVec;
    uint32_t count;
    const char **extensions = glfwGetRequiredInstanceExtensions(&count);

    for (int i = 0; i < count; i++)
        ExtensionVec.push_back(extensions[i]);
    return ExtensionVec;
}