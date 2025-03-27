#include "Image.h"
#include <VK/Devices/LogicalDevice.h>
Image::Image(const ImageProperties &properties) : properties(properties)
{
	MakeImage();
}

Image::Image(const vk::Image &_image, const ImageProperties &ci)
{
	image = _image;
	properties = ci;
}

void Image::Destroy()
{
	GetDevice()->destroyImage(image);
	GetDevice()->freeMemory(imageMemory);
}

void *Image::MapMemory()
{
	return GetDevice()->mapMemory(imageMemory, 0, memorysize).value;
}
void Image::UnmapMemory()
{
	GetDevice()->unmapMemory(imageMemory);
}



void Image::MakeImage()
{
	const bool hasZ = properties.dimensions.z > 1;
	const bool hasY = properties.dimensions.y > 1;
	const bool HasLayers = properties.layers > 1;
	if (hasZ && hasY && HasLayers)
		logger.Error("Invalid Image: cannot create a 3D Texture Array");

	vk::ImageCreateInfo image_create_info;
	image_create_info.sType = vk::StructureType::eImageCreateInfo;
	image_create_info.format = static_cast<vk::Format>(properties.format);
	image_create_info.arrayLayers = properties.layers;
	image_create_info.extent = vk::Extent3D(properties.dimensions.x, properties.dimensions.y, properties.dimensions.z);
	image_create_info.mipLevels = properties.mipLevels;
	image_create_info.tiling = static_cast<vk::ImageTiling>(properties.memoryTilling);
	image_create_info.initialLayout = vk::ImageLayout::eUndefined;
	image_create_info.usage = properties.usage;
	image_create_info.sharingMode = vk::SharingMode::eExclusive;
	image_create_info.samples = static_cast<vk::SampleCountFlagBits>(static_cast<uint32_t>(vk::SampleCountFlagBits::e1) << (properties.Samples - 1));
	image_create_info.imageType = properties.dimensions.y > 1 ? (properties.dimensions.z > 1 ? vk::ImageType::e3D : vk::ImageType::e2D) : vk::ImageType::e1D;
	image = GetDevice()->createImage(image_create_info).value;

	vk::MemoryRequirements memory_requirements = GetDevice()->getImageMemoryRequirements(image);
	vk::MemoryAllocateInfo memory_allocate_info;
	memory_allocate_info.allocationSize = memory_requirements.size;
	memorysize = memory_requirements.size;
	auto supportedMemoryType = GetDevice().GetMemoryTypeThatFits(memory_requirements.memoryTypeBits,
		properties.memory_properties);
	memory_allocate_info.memoryTypeIndex = supportedMemoryType.index;
	if (!supportedMemoryType.isValid())
	{
		logger.Error("No compatible memory type found in " + GetDevice().GetName());
		// logger.Error() << "No compatible memory type found in " << GetDevice().GetName() << Log::end;
		return;
	}

	imageMemory = GetDevice()->allocateMemory(memory_allocate_info).value;

	auto result = GetDevice()->bindImageMemory(image, imageMemory, 0);

	logger.Debug("Image Created. TODO: make this more descriptive");
}
