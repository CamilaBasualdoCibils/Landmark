#include "VKQueue.hpp"
#include "Graphics/Vulkan/VKDevice.hpp"
VK::Queue::Queue(const VK::QueueProperties &Properties) : QueueIndex(Properties.QueueIndex), Family(Properties.Family)
{

    Handle = Properties.Device->GetHandle().getQueue(Properties.Family.FamilyIndex, Properties.QueueIndex);
}