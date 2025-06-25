#include "Image.h"
#include <VK/Devices/LogicalDevice.h>
#include <VK/Operations/CommandBuffer.h>
#include <VK/Buffers/Buffer.h>
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

void Image::TransferLayout(CommandBuffer &cmdbuf, ImageLayouts old_layout, ImageLayouts new_layout)
{
	vk::ImageMemoryBarrier barrier{};
    barrier.oldLayout = (vk::ImageLayout)old_layout;
    barrier.newLayout = (vk::ImageLayout)new_layout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = properties.mipLevels;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = properties.layers;

    vk::PipelineStageFlags sourceStage;
    vk::PipelineStageFlags destinationStage;

    if (old_layout == ImageLayouts::UNDEFINED && new_layout == ImageLayouts::TRANSFER_DST)
    {
        barrier.srcAccessMask = vk::AccessFlagBits::eNone;
        barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
        sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
        destinationStage = vk::PipelineStageFlagBits::eTransfer;
    }
    else if (old_layout == ImageLayouts::TRANSFER_DST && new_layout == ImageLayouts::SHADER_READ_ONLY)
    {
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
        sourceStage = vk::PipelineStageFlagBits::eTransfer;
        destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
    }
    else
    {
        throw std::invalid_argument("Unsupported layout transition");
    }

    cmdbuf->pipelineBarrier(
        sourceStage, destinationStage, {}, nullptr, nullptr, barrier);
}

void Image::CopyFromBuffer(CommandBuffer &cmdbuf, Buffer &buffer)
{
	vk::BufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;  // Tightly packed
    region.bufferImageHeight = 0; // Tightly packed

    region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = vk::Offset3D{0, 0, 0};
    region.imageExtent = vk::Extent3D{properties.dimensions.x, properties.dimensions.y, 1};
	cmdbuf->copyBufferToImage(buffer,image,(vk::ImageLayout)ImageLayouts::TRANSFER_DST,{region});
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

	//logger.Debug("Image Created. TODO: make this more descriptive");
}
