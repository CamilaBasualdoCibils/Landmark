#pragma once
#include <pch.h>
#include "Types/Flags.hpp"
#include <Graphics/GPURef.hpp>
namespace VK
{
    class Device;
enum QueueCapabilities : std::underlying_type_t<vk::QueueFlagBits>
{
    eGraphics = (size_t)vk::QueueFlagBits::eGraphics,
    eCompute = (size_t)vk::QueueFlagBits::eCompute,
    eTransfer = (size_t)vk::QueueFlagBits::eTransfer,
    eSparseBinding = (size_t)vk::QueueFlagBits::eSparseBinding,
    eGeneric = eGraphics | eCompute | eTransfer | eSparseBinding
};
struct QueueFamily
{
    Flags<QueueCapabilities> Capabilities;
    uint32_t FamilyIndex;
};
struct QueueProperties
{
    GPURef<VK::Device> Device;
    uint32_t QueueIndex;
    QueueFamily Family;
};
class Queue
{
    const uint32_t QueueIndex;
    vk::Queue Handle;
    const QueueFamily Family;
    public:
    Queue(const QueueProperties& Properties);

    operator vk::Queue() const {return Handle;}
    operator vk::Queue() {return Handle;}
    [[nodiscard]] auto GetHandle() const {return Handle;}
    [[nodiscard]] auto GetHandle() {return Handle;}

    [[nodiscard]] auto GetFamily() const {return Family;}
    [[nodiscard]] auto GetQueueIndex() const {return QueueIndex;}
};

} // namespace VK