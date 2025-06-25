#include "Window.hpp"
#include "IO/GLFW/GLFW.hpp"

Window::Window()
{

    GLFW::Check();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, true);
    glfwWindowHint(GLFW_DECORATED, true);

    glfwWindowHandle = glfwCreateWindow(1920, 1080, "Default Name", nullptr, nullptr);
    if (!glfwWindowHandle)
        throw std::runtime_error("Failed to create glfw window");

    glfwSetWindowUserPointer(glfwWindowHandle, this);
}