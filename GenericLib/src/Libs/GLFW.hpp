//
// Created by camila on 11/10/25.
//

#ifndef LANDMARK_GLFW_HPP
#define LANDMARK_GLFW_HPP

#define GLFW_INCLUDE_VULKAN
// #define VK_USE_PLATFORM_WIN32_KHR
#include "Utility/Singleton.hpp"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <boost/describe.hpp>
#include <iostream>
#include <unordered_map>
class GLFW : public Singleton<GLFW> {

  enum class Platforms {
    eWin32 = GLFW_PLATFORM_WIN32,
    eX11 = GLFW_PLATFORM_X11,
    eWayland = GLFW_PLATFORM_WAYLAND,
    eCocoa = GLFW_PLATFORM_COCOA
  };
  BOOST_DESCRIBE_NESTED_ENUM(Platforms, eWin32,eCocoa, eX11, eWayland )
public:
  struct Settings {};
  GLFW(const Settings &settings) {

    bool init = false;
    boost::mp11::mp_for_each<boost::describe::describe_enumerators<Platforms>>(
        [&init = init](auto D) {
          if (init)
            return;
          glfwInitHint(GLFW_PLATFORM, (int)D.value);
          const auto result = glfwInit();
          if (result == GLFW_TRUE) {
            std::cout << "GLFW using: " << D.name << std::endl;
            init = true;
            return;
          } else {
            std::cout << "GLFW init failed with: " << D.name << std::endl;
          }
        });
        if (!init)
        {
            throw std::runtime_error("Failed to init GLFW");
        }
  }
};

#endif // LANDMARK_GLFW_HPP