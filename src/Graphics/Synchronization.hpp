#pragma once
#include "Graphics/Interop/Interop.hpp"
#include "Graphics/OpenGL/Synchronization/GLSemaphore.hpp"
#include "Graphics/Vulkan/Synchronization/VKSemaphore.hpp"
#include <pch.h>
namespace Graphics
{
using Semaphore = InteropObject<VK::Semaphore, GL::Semaphore, InteropTransactionType::eVK2GL>;
}