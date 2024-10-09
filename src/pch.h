// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H
#include <assert.h>

#define LASSERT(a,b) assert(a && b)


// add headers that you want to pre-compile here
//#include "framework.h"

//VULKAN
#define VULKAN_HPP_ASSERT_ON_RESULT(x)
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>
//#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_to_string.hpp>


//GLFW
#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
//#define GLFW_EXPOSE_NATIVE_WIN32
//#include <GLFW/include/GLFW/glfw3native.h>


//OPENAL
#include <AL/al.h>


//IMGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_vulkan.h>


#include <implot/implot.h>

//GLM
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/constants.hpp>
using namespace glm;


//JSON
#include <JSON/json.hpp>


#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <fstream>
#include <filesystem>
#include <queue>
#include <type_traits>
#include <stdint.h>
#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <variant>
#include <thread>
#include <mutex>
#endif //PCH_H
