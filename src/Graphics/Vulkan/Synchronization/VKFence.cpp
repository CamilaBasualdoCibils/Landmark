#include "VKFence.hpp"
#include "Graphics/GraphicsEngine.hpp"

VK::Fence::Fence()
{
    Handle = GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createFence({}).value;
}

VK::Fence::~Fence()
{
    GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().destroy(Handle);
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