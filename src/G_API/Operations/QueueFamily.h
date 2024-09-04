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
	const vk::PhysicalDevice owner;
	const FamilyID_T familyID;

	const vk::QueueFamilyProperties familyProperties;
	const vk::QueueFlags queueFlags;

public:
	
	const uint32_t queueCount;

	const Capabilities capabilities;
	const uint32_t timeStampValidBits;
	const vk::Extent3D minImageTransferGranularity;


	QueueFamily(vk::PhysicalDevice device, const uint32_t& familyID);

	QueueFamily(const QueueFamily& o) = default;
	
	FamilyID_T GetID() const { return familyID; }
private:
	Capabilities FetchCapabilities(const uint32_t& familyID);
};
#endif