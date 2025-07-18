#pragma once
#include <glm/fwd.hpp>
#include <pch.h>
#include <vulkan/vulkan_structs.hpp>


inline uvec2 VkToGlm(vk::Extent2D e) {return uvec2(e.width,e.height);}
inline vk::Offset2D GlmToVkOffset(ivec2 e) {return vk::Offset2D{e.x,e.y};}
inline vk::Extent2D GlmToVkExtent(uvec2 e) {return vk::Extent2D(e.x,e.y);}
inline vk::Rect2D GlmToVkRect(uvec2 offset, uvec2 size) {return vk::Rect2D(GlmToVkOffset(offset),GlmToVkExtent(size));}

inline uvec3 VkToGlm(vk::Extent3D e) {return uvec3(e.width,e.height,e.depth);}
inline vk::Extent3D GlmToVkExtent(uvec3 e) {return vk::Extent3D(e.x,e.y,e.z);}
inline vk::Offset3D GlmToVkOffset(ivec3 e) {return vk::Offset3D{e.x,e.y,e.z};}