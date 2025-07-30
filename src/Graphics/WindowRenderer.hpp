#pragma once
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/Vulkan/VKSwapchain.hpp"
#include "IO/Window/Window.hpp"
#include <pch.h>

class WindowRenderer
{
std::shared_ptr<Window> Window;
GPURef<VK::Swapchain> swapchain;
};