#include <pch.h>
#pragma once
#include <Editor/Logging/Log.h>
#include <VK/Rendering/Swapchain.h>

class GLFW_Instance
{
    static inline std::unique_ptr<GLFW_Instance> instance;
    static inline Log logger = Log("GLFW");

public:
    GLFW_Instance()
    {
        if (glfwInit() != GLFW_TRUE)
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        logger.Debug("Init");
        glfwSetErrorCallback(Error_Callback);
    }
    ~GLFW_Instance()
    {
        glfwTerminate();
        logger.Debug("Glfw terminated");
    }

private:
    static void Error_Callback(int code, const char *description)
    {
        logger.Debug(description);
    };

public:
    static inline GLFW_Instance *GetInstance()
    {
        if (!instance.get())
            instance = std::make_unique<GLFW_Instance>();
        return instance.get();
    }
};
class LWindow
{
public:
using internal_window_handle = Window;
    struct LWindow_Properties
    {

        std::string window_name = "Default Window Name";
        bool resizable = true;
        bool decoration = true;
        bool transparent = false;
        uvec2 window_size = {2560, 1400};
        LWindow_Properties() = default;
    };

private:
    GLFWwindow *window_ptr = nullptr;
    LWindow_Properties properties;
    static inline Log logger = Log("LWindow");
    bool surface_created = false;
    vk::SurfaceKHR surface;
    
public:
    LWindow(LWindow_Properties _properties);
    vk::SurfaceKHR GetorMakeSurface(vk::Instance _instance);

    glm::uvec2 GetSize() const {return properties.window_size;}
    vk::SurfaceKHR GetSurface() const;
    bool isClosing() const { return glfwWindowShouldClose(window_ptr); }
    ~LWindow();
    internal_window_handle GetInternalHandle();
    GLFWwindow* GetGlfwHandle() {return window_ptr;}

private:
    void CreateWindow();
};
