#pragma once
#ifndef _LOGICALDEVICE_H_
#define _LOGICALDEVICE_H_

#include "PhysicalDevice.h"

#include "Debug/Debug.h"
#include "VK/Operations/Queue.h"

struct QueueRequest
{
	uint32_t count;
	QueueFamily::Capabilities capabilities;
	// float Priority = 1.0f;
};
struct DeviceFeatures
{
	std::vector<QueueRequest> queues;

	//features
	bool geometryShader = false;
	bool wideLines = false;
	bool tesselationShader = false;
	bool sampleRateShading = false;
	bool samplerAnistropy = false;
	
	//extensions
	bool swapchainSupport = false;
	bool descriptorIndexding = false; //bindless descriptors
	bool meshShader = false;
	bool raytracing = false;

	bool khronosValidation = false;

};

class LogicalDevice : public PhysicalDevice
{
	

public:
private:
	Log log;
	const DeviceFeatures features;
	//const std::vector<std::string> requestedExtensions;
	//const std::vector<std::string> requestedLayers;
	//const vk::PhysicalDeviceFeatures requestedFeatures;

	// Capabilities requested for queues
	using QueueFamilyRequestInfo = std::vector<QueueFamily::Capabilities::FlagMask>;

	// Requested Capabilities sorted into families
	using QueueFamilyCountContainer = std::map<QueueFamily::FamilyID_T, QueueFamilyRequestInfo>;
	const QueueFamilyCountContainer QueueCountPerFamily;

	// Final Queues sorted by requested capacity. a
	using QueueList = std::vector<Queue *>;
	using QueueContainer = std::map<QueueFamily::Capabilities::FlagMask, QueueList>;

	vk::Device device;

	QueueContainer queues;

public:
	LogicalDevice(const PhysicalDevice &pd,const DeviceFeatures& features);
	vk::Device *operator->() { return &device; }
	vk::Device &GetVkDevice() { return device; }
	// const QueueList& GetQueues(QueueFamily::Capabilities::FlagMask capabilities);
	const std::vector<Queue *> &GetQueues(QueueFamily::Capabilities::FlagMask capabitlies);
	operator vk::Device() const { return device; };
	// bool operator==(const LogicalDevice&) const;
	// bool operator!=(const LogicalDevice& o) const;
	vk::Device GetvkDevice() { return device; }
	// MemoryTypeIndexInfo GetMemoryTypeThatFits(uint32_t typefilter, uint64_t bits);
private:
	vk::Device CreateDevice();

	const QueueFamilyCountContainer DeduceQueueFamilies(const std::vector<QueueRequest> &queue_requests);
	void RetrieveQueues();

protected:
	
};
#endif
