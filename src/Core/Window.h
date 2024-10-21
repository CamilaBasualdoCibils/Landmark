#include <pch.h>
#include <Debug/Logging/Log.h>
#include <VK/Rendering/Swapchain.h>

class GLFW_Instance
{
    static inline GLFW_Instance *instance = nullptr;
    static inline Log logger = Log("GLFW");
    GLFW_Instance()
    {
        if (glfwInit() != GLFW_TRUE)
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        logger.Debug("Init");
        glfwSetErrorCallback(Error_Callback);
    }
    private:
    static void Error_Callback(int code, const char* description) {
        logger.Debug(description);
    };

public:
    static inline GLFW_Instance *GetInstance() { return instance ? instance : instance = new GLFW_Instance(); }
};
class Window
{
public:
    struct Window_Properties
    {

        std::string window_name = "Default Window Name";
        bool resizable = true;
        bool decoration = true;
        uvec2 window_size = {1920, 1080};
        Window_Properties() = default;
    };

private:
    GLFWwindow *window_ptr = nullptr;
    Window_Properties properties;
    static inline Log logger = Log("Window");
    bool surface_created = false;
    vk::SurfaceKHR surface;
    std::optional<Swapchain> swapchain;
public:
    Window( Window_Properties _properties);
    vk::SurfaceKHR GetorMakeSurface(vk::Instance _instance);
    
    constexpr vk::SurfaceKHR GetSurface() const ;
    bool isClosing() const {return glfwWindowShouldClose(window_ptr);}
    ~Window();
private:
    void CreateWindow();
};