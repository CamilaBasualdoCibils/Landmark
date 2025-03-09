#include "pch.h"
#include "LogicalDevice.h"

#include <bitset>

LogicalDevice::LogicalDevice(const PhysicalDevice &pd, const DeviceFeatures &_features) : PhysicalDevice(pd),
																						  log(Name),
																						  features(_features),
																						  QueueCountPerFamily(DeduceQueueFamilies(features.queues)),
																						  device(CreateDevice())
{
	RetrieveQueues();
}
uint8_t countSetBits(uint8_t n)
{
	uint8_t count = 0;
	while (n)
	{
		count += n & 1;
		n >>= 1;
	}
	return count;
}

const std::vector<Queue *> &LogicalDevice::GetQueues(QueueFamily::Capabilities::FlagMask capabilities)
{

	if (!queues.contains(capabilities))

	{
		std::stringstream ss;
		ss << "No queue found with capability" << std::bitset<8>(capabilities);
		log.Critical(ss.str());
	}
	/*
	std::set<Queue> q = queues[capabilities];
	return q;
	//for (auto& queue : queues[capabilities])
	//	q.emplace(queue);
	*/

	return queues[capabilities];
}
static int CountBits(int n)
{
	int count = 0;
	while (n)
	{
		count += n & 1;
		n >>= 1;
	}
	return count;
}

/*
LogicalDevice::MemoryTypeIndexInfo LogicalDevice::GetMemoryTypeThatFits(uint32_t typefilter, uint64_t bits)
{
	return GetMemoryTypeThatFits(typefilter,static_cast<vk::MemoryPropertyFlags>(static_cast<VkMemoryPropertyFlags>(bits)));
}
*/

vk::Device LogicalDevice::CreateDevice()
{
	std::vector<const char *> _Layers;
	std::vector<const char *> _extensions;
		const std::vector<float> priorities(32, 1.0f);
	std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
	{

		for (auto &finalFamily : QueueCountPerFamily)
		{
			vk::DeviceQueueCreateInfo queue_create{};
			queue_create.queueCount = finalFamily.second.size();
			queue_create.queueFamilyIndex = finalFamily.first;
			queue_create.pQueuePriorities = priorities.data();

			queue_create_infos.push_back(queue_create);
		}
	}
	vk::PhysicalDeviceDescriptorIndexingFeatures descriptor_indexing_features{};

	vk::PhysicalDeviceRayTracingPipelineFeaturesKHR raytracing_features{};
	raytracing_features.pNext = &descriptor_indexing_features;

	if (features.descriptorIndexding)
	{
		_extensions.push_back(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);
		descriptor_indexing_features.descriptorBindingPartiallyBound = true;
		descriptor_indexing_features.descriptorBindingSampledImageUpdateAfterBind = true;
		descriptor_indexing_features.descriptorBindingStorageBufferUpdateAfterBind = true;
		descriptor_indexing_features.descriptorBindingStorageImageUpdateAfterBind = true;
		descriptor_indexing_features.descriptorBindingStorageTexelBufferUpdateAfterBind = true;
		descriptor_indexing_features.descriptorBindingUniformBufferUpdateAfterBind = true;
		descriptor_indexing_features.descriptorBindingUniformTexelBufferUpdateAfterBind = true;
		descriptor_indexing_features.descriptorBindingUpdateUnusedWhilePending = true;
		descriptor_indexing_features.descriptorBindingVariableDescriptorCount = true;
		descriptor_indexing_features.runtimeDescriptorArray = true;
		descriptor_indexing_features.shaderInputAttachmentArrayDynamicIndexing = true;
		descriptor_indexing_features.shaderInputAttachmentArrayNonUniformIndexing = true;
		descriptor_indexing_features.shaderSampledImageArrayNonUniformIndexing = true;
		descriptor_indexing_features.shaderStorageBufferArrayNonUniformIndexing = true;
		descriptor_indexing_features.shaderStorageImageArrayNonUniformIndexing = true;
		descriptor_indexing_features.shaderStorageTexelBufferArrayDynamicIndexing = true;
		descriptor_indexing_features.shaderStorageTexelBufferArrayNonUniformIndexing = true;
		descriptor_indexing_features.shaderUniformBufferArrayNonUniformIndexing = true;
		descriptor_indexing_features.shaderUniformTexelBufferArrayDynamicIndexing = true;
		descriptor_indexing_features.shaderUniformTexelBufferArrayNonUniformIndexing = true;
	}

	if (features.swapchainSupport)
		_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	if (features.meshShader)
		_extensions.push_back(VK_EXT_MESH_SHADER_EXTENSION_NAME);

	if (features.raytracing)
	{
		_extensions.push_back(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);

		raytracing_features.rayTracingPipeline = true;
		raytracing_features.rayTracingPipelineTraceRaysIndirect = true;
		raytracing_features.rayTraversalPrimitiveCulling = true;
		_extensions.insert(_extensions.end(), PhysicalDevice::rayTracingExtensions.begin(), PhysicalDevice::rayTracingExtensions.end());
	}

	{
		auto checkPush = [&](bool v, const char *ext)
		{if (v) _Layers.push_back(ext); };
		checkPush(features.khronosValidation, "VK_LAYER_KHRONOS_validation");
	}
	vk::PhysicalDeviceFeatures device_features{};
	{
		device_features.geometryShader = features.geometryShader;
		device_features.wideLines = features.wideLines;
		device_features.tessellationShader = features.tesselationShader;
		device_features.sampleRateShading = features.sampleRateShading;
		device_features.samplerAnisotropy = features.samplerAnistropy;
	}
	vk::DeviceCreateInfo device_create_info{};
	device_create_info.ppEnabledExtensionNames = _extensions.data();
	device_create_info.enabledExtensionCount = _extensions.size();

	device_create_info.ppEnabledLayerNames = _Layers.data();
	device_create_info.enabledLayerCount = _Layers.size();
	device_create_info.pQueueCreateInfos = queue_create_infos.data();
	device_create_info.queueCreateInfoCount = queue_create_infos.size();
	device_create_info.pEnabledFeatures = &device_features;
	device_create_info.pNext = &raytracing_features;
	auto result = physicalDevice.createDevice(device_create_info);
	device = result.value;
	if (result.result != vk::Result::eSuccess)
	{
		log.Critical(std::string("Failed to create Logical Device. error: \n"));
	}
	std::stringstream ss;
	ss << "\n====== Logical Device Init ======\n";
	ss << "Extensions:\n";
	for (auto &ext : _extensions)
		ss << "	" << ext << "\n";
	ss << "Layers:\n";
	for (auto &layer : _Layers)
		ss << "	" << layer << "\n";

	ss << "Queues: \n";
	ss << "  Family_ID : Count\n";
	for (auto &family : QueueCountPerFamily)
		ss << '	' << family.first << " : " << family.second.size() << "\n";
	ss << "=================================";
	log.Debug(ss.str());
	return device;
}

const LogicalDevice::QueueFamilyCountContainer LogicalDevice::DeduceQueueFamilies(const std::vector<QueueRequest> &queue_requests)
{
	QueueFamilyCountContainer FinalCountPerFamily;
	{
		struct QueueFamilyAvailableInfo
		{
			QueueFamilyAvailableInfo(const QueueFamily &q) : family(q)
			{
				availableCount = family.queueCount;
			}
			const QueueFamily &family;
			int availableCount;
		};
		// std::vector<QueueFamilyAvailableInfo> availableQueueFamilies;
		//        V family ID
		std::map<uint32_t, QueueFamilyAvailableInfo> AvailableQueueFamilies;
		// std::multimap<QueueFamily::Capabilities::FlagMask, QueueFamilyAvailableInfo> _availableQueueFamilies;
		for (auto &queuefamily : queueFamilies)
		{
			// availableQueueFamilies.emplace_back(queuefamily);
			AvailableQueueFamilies.insert(std::make_pair(queuefamily.GetID(), QueueFamilyAvailableInfo(queuefamily)));
		}
		// every queue request
		for (auto &request : queue_requests)
		{
			const auto requestMask = request.capabilities.ToFlagMask();
			uint64_t bestFitFamily = -1;
			uint8_t bestFitDifference = UINT8_MAX;
			// find best fit queue family
			for (auto &available : AvailableQueueFamilies)
			{

				const auto availableMask = available.second.family.capabilities.ToFlagMask();

				if ((requestMask & availableMask) != requestMask)
					continue; // INCOMPATIBLE

				if (requestMask == availableMask) // perfect fit
				{
					bestFitFamily = available.first;
					bestFitDifference = 0;
					break;
				}

				QueueFamily::Capabilities::FlagMask leftover = availableMask & (~requestMask);

				uint8_t currentDifferece = countSetBits(leftover);
				if (currentDifferece < bestFitDifference)
				{
					bestFitDifference = currentDifferece;
					bestFitFamily = available.first;
				}
			}

			const QueueFamily chosenQueueFamily = AvailableQueueFamilies.at(bestFitFamily).family;
			QueueFamilyAvailableInfo &availableInfo = AvailableQueueFamilies.at(bestFitFamily);
			availableInfo.availableCount--;
			if (!(availableInfo.availableCount))
			{
				AvailableQueueFamilies.erase(bestFitFamily);
			}
			if (!FinalCountPerFamily.contains(chosenQueueFamily.GetID()))
				FinalCountPerFamily[chosenQueueFamily.GetID()] = {};

			FinalCountPerFamily[chosenQueueFamily.GetID()];
			FinalCountPerFamily[chosenQueueFamily.GetID()].push_back(request.capabilities.ToFlagMask());
		}
	}

	return FinalCountPerFamily;
}

void LogicalDevice::RetrieveQueues()
{

	// return container;
	for (auto &queueFamily : QueueCountPerFamily)
	{

		int i = 0;
		for (auto &request : queueFamily.second)
		{
			if (!queues.contains(request))
				queues[request] = {};

			// auto vkQueue = device.getQueue(queueFamily.first, i);

			queues[request].emplace_back(new Queue(*this, queueFamily.first, i));
			i++;
		}
	}
}
