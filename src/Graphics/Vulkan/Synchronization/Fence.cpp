#include "Fence.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>

VK::Fence::Fence()
{
    Handle = GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createFence({}).value;
}

void VK::Fence::Wait()
{

    auto result = GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().waitForFences(
        {Handle}, VK_TRUE, UINT64_MAX);
}

void VK::Fence::Reset()
{
    GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().resetFences({Handle});
}

void VK::Fence::WaitAndReset()
{
    Wait();
    Reset();
}

bool VK::Fence::IsSignaled() const
{
    auto result = GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().getFenceStatus(Handle);

    return result == vk::Result::eSuccess;
}