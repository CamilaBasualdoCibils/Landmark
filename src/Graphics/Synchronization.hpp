#pragma once
#include "Graphics/Interop/Interop.hpp"
#include "Graphics/OpenGL/Synchronization/Semaphore.hpp"
#include "Graphics/Vulkan/Synchronization/Semaphore.hpp"
#include <pch.h>
namespace Graphics
{
using Semaphore = InteropObject<VK::Semaphore, GL::Semaphore, InteropTransactionType::eVK2GL>;
}