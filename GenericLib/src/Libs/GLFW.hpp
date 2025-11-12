//
// Created by camila on 11/10/25.
//

#ifndef LANDMARK_GLFW_HPP
#define LANDMARK_GLFW_HPP

#define GLFW_INCLUDE_VULKAN
//#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "Utility/Singleton.hpp"

class GLFW : public Singleton<GLFW> {

public:
    struct Settings {};
    GLFW(const Settings& settings) {
        const auto result = glfwInit();
        LASSERT(result == GLFW_TRUE,"Failed to start GLFW");
    }

};

#endif //LANDMARK_GLFW_HPP