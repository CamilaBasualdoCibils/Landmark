#include <pch.h>
#include "Window.h"



Window::Window( Window_Properties _p) : properties(_p)
{
    CreateWindow();
}

vk::SurfaceKHR Window::GetorMakeSurface(vk::Instance _instance)
{
    if (!surface_created)
    {
        VkSurfaceKHR surf;
        glfwCreateWindowSurface(_instance, window_ptr, nullptr, &surf);
        logger.Debug("Surface Created");
        surface = vk::SurfaceKHR(surf);
    }
    return surface;
}

constexpr vk::SurfaceKHR Window::GetSurface() const
{
    LASSERT(surface_created,"Surface not created");
    return surface;
}

Window::~Window()
{
    glfwDestroyWindow(window_ptr);
    logger.Debug("Window Destroyed");
}

void Window::CreateWindow()
{
    if (window_ptr)
        throw std::runtime_error("Window Recycling not implemented");
    GLFW_Instance::GetInstance();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, properties.resizable);
    glfwWindowHint(GLFW_DECORATED, properties.decoration);

    window_ptr = glfwCreateWindow(properties.window_size.x, properties.window_size.y,
                                  properties.window_name.c_str(), nullptr, nullptr);
    if (!window_ptr)
        throw std::runtime_error("Failed to create glfw window");

    glfwSetWindowUserPointer(window_ptr, this);
    logger.Debug("Window Created");
}
