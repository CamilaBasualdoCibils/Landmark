// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H
#include <assert.h>

#define LASSERT(a, b) assert(a &&b)

// add headers that you want to pre-compile here
// #include "framework.h"

// VULKAN
#define VULKAN_HPP_ASSERT_ON_RESULT(x)
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>
// #include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_to_string.hpp>

// EGL

#include <EGL/egl.h>
#include <EGL/eglext.h>
// GLEW
#include <GL/glew.h>
#include <GL/gl.h>
// GLFW
#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// IMGUI
// #define IMGUI_USE_WCHAR32
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_vulkan.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imstb_truetype.h>


#include <implot/implot.h>

// GLM
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

// JSON
//#include <JSON/json.hpp>
//using Json = nlohmann::json;
//using Json_Ordered = nlohmann::ordered_json;
namespace stb
{
#include "stb/stb_image.h"

}

#include <algorithm>
#include <cctype>
#include <chrono>
#include <execution>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <random>
#include <ranges>
#include <set>
#include <sstream>
#include <stdint.h>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#endif // PCH_H
