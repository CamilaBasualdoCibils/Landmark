#pragma once
#ifndef _QUEUEFAMILY_H_
#define _QUEUEFAMILY_H_
class QueueFamily
{
public:
	using FamilyID_T = uint32_t;
	struct Capabilities
	{
		bool Graphics = false;
		bool Present = false;
		bool Compute = false;
		bool Transfer = false;
		bool SparseBinding = false;

		using FlagMask = uint8_t;

		static inline const FlagMask GraphicsMask = 0b1;
		static inline const FlagMask PresentMask = 0b10;
		static inline const FlagMask ComputeMask = 0b100;
		static inline const FlagMask TransferMask = 0b1000;
		static inline const FlagMask SparseBindingMask = 0b10000;

		FlagMask ToFlagMask() const
		{
			return (GraphicsMask * Graphics) | (PresentMask * Present)
			| (ComputeMask * Compute) | (TransferMask * Transfer) | (SparseBindingMask * SparseBinding);
		}
	};
private:
	vk::PhysicalDevice owner;
	FamilyID_T familyID;

	vk::QueueFamilyProperties familyProperties;
	vk::QueueFlags queueFlags;

public:
	
	uint32_t queueCount;

	Capabilities capabilities;
	uint32_t timeStampValidBits;
	vk::Extent3D minImageTransferGranularity;


	QueueFamily(vk::PhysicalDevice device, const uint32_t& familyID,vk::SurfaceKHR* surface);

	QueueFamily(const QueueFamily& o) = default;
	
	FamilyID_T GetID() const { return familyID; }
private:
	Capabilities FetchCapabilities(const uint32_t& familyID,vk::SurfaceKHR* surface);
};
#endif