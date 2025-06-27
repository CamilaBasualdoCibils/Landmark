#pragma once
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/Vulkan/Swapchain.hpp"
#include "IO/Window/Window.hpp"
#include <pch.h>

class WindowRenderer
{
std::shared_ptr<Window> Window;
GPURef<VK::Swapchain> swapchain;
};