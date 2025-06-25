#pragma once
#ifndef _PHYSICALDEVICE_H_
#define _PHYSICALDEVICE_H_
#include "GenericEnums.h"
#include "VK/Operations/QueueFamily.h"
enum class device_features : uint64_t
{
	RAY_TRACING = 1 << 0,
	PRESENTABLE = 1 << 1
};
enum class device_parameters : uint64_t
{
	MEMORY = 1 << 0
};
class PhysicalDevice
{
private:
protected:
	vk::PhysicalDevice physicalDevice;
	vk::PhysicalDeviceProperties properties;
	vk::PhysicalDeviceFeatures features;
	vk::PhysicalDeviceMemoryProperties memoryProperties;

	std::string Name;
	uint32_t UniqueID;
	vk::PhysicalDeviceType type;
	std::vector<QueueFamily> queueFamilies;

	std::map<std::string, vk::ExtensionProperties> AvailableExtensions;

public:
	PhysicalDevice(const vk::PhysicalDevice &pd, vk::SurfaceKHR *surface = nullptr);
	PhysicalDevice(const PhysicalDevice &o) = default;
	PhysicalDevice &operator=(const PhysicalDevice &o) = default;
	operator vk::PhysicalDevice() const { return physicalDevice; }

	vk::PhysicalDevice &GetVkPhyDevice() { return physicalDevice; }
	const std::string &GetName() const { return Name; }
	uint32_t GetUniqueID() const { return UniqueID; }
	const vk::PhysicalDeviceMemoryProperties &GetMemoryProperties() const { return memoryProperties; }

	vk::SurfaceCapabilitiesKHR GetSurfaceCapabilities(vk::SurfaceKHR surface) const { return physicalDevice.getSurfaceCapabilitiesKHR(surface).value; }
	std::vector<vk::PresentModeKHR> GetPresentModes(vk::SurfaceKHR surface) const { return physicalDevice.getSurfacePresentModesKHR(surface).value; }
	std::vector<vk::SurfaceFormatKHR> GetSurfaceFormats(vk::SurfaceKHR surface) const { return physicalDevice.getSurfaceFormatsKHR(surface).value; }
	bool HasExtension(const std::string &name) const;
	const vk::ExtensionProperties &GetExtension(const std::string &name) const;

	struct MemoryTypeIndexInfo
	{
		int index;
		vk::MemoryPropertyFlags flags;

		bool isValid() const { return index >= 0; }
	};

	MemoryTypeIndexInfo GetMemoryTypeThatFits(uint32_t typefilter, Flags<MemoryProperties> bits);
	bool SupportsRayTracing() const
	{
		for (auto &ext : rayTracingExtensions)
			if (!AvailableExtensions.contains(ext))
				return false;

		return true;
	}

	const static inline std::vector<const char *> rayTracingExtensions = {
		VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
		VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
		VK_KHR_RAY_QUERY_EXTENSION_NAME,
		VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
		VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME};

private:
	std::vector<QueueFamily> FetchQueueFamilies(vk::SurfaceKHR *surface);
	std::map<std::string, vk::ExtensionProperties> FetchExtensions();
};
#endif
