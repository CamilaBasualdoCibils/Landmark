#include "GLFW.hpp"

GLFW::GLFW()
{
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    if (glfwInit() != GLFW_TRUE)
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwSetErrorCallback(ErrorCallback);
    int Major, Minor, Rev;
    glfwGetVersion(&Major, &Minor, &Rev);
    std::string VersionString = std::format("GLFW {}.{}.{}", Major, Minor, Rev);

    switch (glfwGetPlatform())
    {
    case GLFW_PLATFORM_X11:
        VersionString += "Using X11";
        break;
    case GLFW_PLATFORM_WAYLAND:
        VersionString += "Using Wayland";
        break;
    default:
        VersionString += "Other platform";
        break;
    }
    std::cout << VersionString << std::endl;
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