#include "Queue.hpp"
#include "Graphics/Vulkan/Device.hpp"
VK::Queue::Queue(const VK::QueueProperties &Properties) : QueueIndex(Properties.QueueIndex), Family(Properties.Family)
{

    Handle = Properties.Device->GetHandle().getQueue(Properties.Family.FamilyIndex, Properties.QueueIndex);
}