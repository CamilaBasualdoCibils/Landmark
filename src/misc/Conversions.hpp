#pragma once
#include <glm/fwd.hpp>
#include <pch.h>
#include <vulkan/vulkan_structs.hpp>


inline uvec2 VkToGlm(vk::Extent2D e) {return uvec2(e.width,e.height);}
inline vk::Extent2D GlmToVk(uvec2 e) {return vk::Extent2D(e.x,e.y);}

inline uvec3 VkToGlm(vk::Extent3D e) {return uvec3(e.width,e.height,e.depth);}
inline vk::Extent3D GlmToVk(uvec3 e) {return vk::Extent3D(e.x,e.y,e.z);}