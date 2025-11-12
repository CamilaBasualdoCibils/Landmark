//
// Created by camila on 11/10/25.
//

#ifndef LANDMARK_GRAPHICS_HPP
#define LANDMARK_GRAPHICS_HPP
#include "Platform/Window.hpp"
#include "Utility/Singleton.hpp"



class Graphics : public Singleton<Graphics> {

std::shared_ptr<Window> _window;
public:
    enum class APIMode {
        eVulkan = 0b1,

        //Opengl but Vulkan for presentation.
        //OpenGL does not play well with multiple windows
        eOpenGL_VK = 0b10,

        //Vulkan shares to OpenGl
        eVulkanAndOpenGL = eVulkan | eOpenGL_VK,

        eDirectX = 0b100,

        //Vulkan shares to DirectX
        eVulkanAndDirectX = eVulkan | eDirectX,

        //Vulkan shares to OpenGL and DirectX
        eVulkanAndOpenGLAndDirectX = eVulkan| eOpenGL_VK | eDirectX
    };
    struct Settings {
        APIMode _APIMode = APIMode::eVulkan;
    };

    void Init(const Settings& settings);


    std::weak_ptr<Window> GetWindow() const {return _window;}

    void NewFrame();
    void EndFrame();
    void Render();
};


#endif //LANDMARK_GRAPHICS_HPP