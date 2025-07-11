#pragma once
#include <pch.h>
#ifndef _GRAPHICS_ENUMS_H_
#define _GRAPHICS_ENUMS_H_
/*
#define _CREATE_ENUM_(x)                                                                                                        \
	enum class x : EnumType;                                                                                                    \
	inline constexpr EnumType operator|(const x &l, const x &r) { return static_cast<EnumType>(l) | static_cast<EnumType>(r); } \
	enum class x : EnumType
*/
#define _CREATE_ENUM_(x, y)                                                                                                     \
	enum class x : EnumType;                                                                                                    \
	inline constexpr EnumType operator|(const x &l, const x &r) { return static_cast<EnumType>(l) | static_cast<EnumType>(r); } \
	constexpr y enum_cast(const x &_a) { return static_cast<y>(_a); }                                                            \
	constexpr x enum_cast(const y &_a) { return static_cast<x>(_a); }                                                            \
	enum class x : EnumType

#include "Types/Flags.hpp"
template <typename T>
static constexpr int int_cast(T t) { return static_cast<int>(t); }

using EnumType = uint32_t;

_CREATE_ENUM_(MemoryTillingType, vk::ImageTiling){
	LINEAR = int_cast(vk::ImageTiling::eLinear),  // store in regular row major order
	OPTIMAL = int_cast(vk::ImageTiling::eOptimal) // stored in what gpu decides is best for performance. reading directly from
												  // will probably return faulty data
};

_CREATE_ENUM_(ShaderStage, vk::ShaderStageFlagBits){
	VERTEX = int_cast(vk::ShaderStageFlagBits::eVertex),
	GEOMETRY = int_cast(vk::ShaderStageFlagBits::eGeometry),
	FRAGMENT = int_cast(vk::ShaderStageFlagBits::eFragment),
	ALL = int_cast(vk::ShaderStageFlagBits::eAll),
};

_CREATE_ENUM_(SharingMode, vk::SharingMode){
	// one queue family can access this object at a time
	EXCLUSIVE = int_cast(vk::SharingMode::eExclusive),

	// no protection, multiple queue families can access at any time
	CONCURRENT = int_cast(vk::SharingMode::eConcurrent),
};

_CREATE_ENUM_(Format, vk::Format){
	RGBA_8_UINT = int_cast(vk::Format::eR8G8B8A8Uint),
	BGRA_8_UNORM = int_cast(vk::Format::eB8G8R8A8Unorm),

	BGRA_8_SRGB = int_cast(vk::Format::eB8G8R8A8Srgb),

	RGBA_8_SRGB = int_cast(vk::Format::eR8G8B8A8Srgb),
	RGB_8_SRGB = int_cast(vk::Format::eR8G8B8Srgb),
	RG_8_SRGB = int_cast(vk::Format::eR8G8Srgb),
	R_8_SRGB = int_cast(vk::Format::eR8Srgb),

	RGBA_16_UNORM = int_cast(vk::Format::eR16G16B16A16Unorm),
	RGB_16_UNORM = int_cast(vk::Format::eR16G16B16Unorm),
	RG_16_UNORM = int_cast(vk::Format::eR16G16Unorm),
	R_16_UNORM = int_cast(vk::Format::eR16Unorm),

	R_32_SFLOAT = int_cast(vk::Format::eR32Sfloat), // scalar 4 byte signed float
	RG_32_SFLOAT = int_cast(vk::Format::eR32G32Sfloat), // vec2 4 byte signed float
	RGB_32_SFLOAT = int_cast(vk::Format::eR32G32B32Sfloat), // vec3 4 byte signed float
	RGBA_32_SFLOAT = int_cast(vk::Format::eR32G32B32A32Sfloat),  // vec4 4 byte signed float
	R_64_SFLOAT = int_cast(vk::Format::eR64Sfloat),
	R_32_INT = int_cast(vk::Format::eR32Sint),
	R_32_UINT = int_cast(vk::Format::eR32Uint),
};

_CREATE_ENUM_(ImageUsage, vk::ImageUsageFlagBits){
	TRANSFER_DST = int_cast(vk::ImageUsageFlagBits::eTransferDst),
	TRANSFER_SRC = int_cast(vk::ImageUsageFlagBits::eTransferSrc),
	COLOR_ATTACHMENT = int_cast(vk::ImageUsageFlagBits::eColorAttachment),
	SAMPLED= int_cast(vk::ImageUsageFlagBits::eSampled)

};

_CREATE_ENUM_(SampleCountFlags, vk::SampleCountFlagBits){
	e1 = int_cast(vk::SampleCountFlagBits::e1),
	e2 = int_cast(vk::SampleCountFlagBits::e2),
	e4 = int_cast(vk::SampleCountFlagBits::e4),
	e8 = int_cast(vk::SampleCountFlagBits::e8),
	e16 = int_cast(vk::SampleCountFlagBits::e16),
	e32 = int_cast(vk::SampleCountFlagBits::e32),
	e64 = int_cast(vk::SampleCountFlagBits::e64),
};
_CREATE_ENUM_(VertexInputRate,vk::VertexInputRate) {
	VERTEX = int_cast(vk::VertexInputRate::eVertex),
	INSTANCE = int_cast(vk::VertexInputRate::eInstance),
};
_CREATE_ENUM_(AttachmentLoadOp, vk::AttachmentLoadOp){
	CLEAR = int_cast(vk::AttachmentLoadOp::eClear),
	DONT_CARE = int_cast(vk::AttachmentLoadOp::eDontCare),
	LOAD = int_cast(vk::AttachmentLoadOp::eLoad),
};

_CREATE_ENUM_(AttachmentStoreOp, vk::AttachmentStoreOp){
	STORE = int_cast(vk::AttachmentStoreOp::eStore),
	NONE = int_cast(vk::AttachmentStoreOp::eNone),
	DONT_CARE = int_cast(vk::AttachmentStoreOp::eDontCare),
};

_CREATE_ENUM_(ImageLayouts, vk::ImageLayout){
	UNDEFINED = int_cast(vk::ImageLayout::eUndefined),
	COLOR_ATTACHMENT = int_cast(vk::ImageLayout::eColorAttachmentOptimal),
	DEPTH_ATTACHMENT = int_cast(vk::ImageLayout::eDepthAttachmentOptimal),
	PRESENT_SRC = int_cast(vk::ImageLayout::ePresentSrcKHR),
	TRANSFER_DST = int_cast(vk::ImageLayout::eTransferDstOptimal),
	TRANSFER_SRC = int_cast(vk::ImageLayout::eTransferSrcOptimal),
	SHADER_READ_ONLY = int_cast(vk::ImageLayout::eShaderReadOnlyOptimal),
};

_CREATE_ENUM_(BufferUsage, vk::BufferUsageFlagBits){
	VERTEX_BUFFER = int_cast(vk::BufferUsageFlagBits::eVertexBuffer),
	UNIFORM_BUFFER = int_cast(vk::BufferUsageFlagBits::eUniformBuffer),
	INDEX_BUFFER = int_cast(vk::BufferUsageFlagBits::eIndexBuffer),
	TRANSFER_DST = int_cast(vk::BufferUsageFlagBits::eTransferDst),
	TRANSFER_SRC = int_cast(vk::BufferUsageFlagBits::eTransferSrc),
};

_CREATE_ENUM_(MemoryProperties, vk::MemoryPropertyFlagBits){
	// specifies that memory allocated with this type is the most efficient for device access
	DEVICE_LOCAL = int_cast(vk::MemoryPropertyFlagBits::eDeviceLocal),
	// specifies that memory allocated with this type can be mapped for host access using vkMapMemory.
	HOST_VISIBLE = int_cast(vk::MemoryPropertyFlagBits::eHostVisible),
	// specifies that the host cache management commands vkFlushMappedMemoryRanges and vkInvalidateMappedMemoryRanges
	// are not needed to flush host writes to the device or make device writes visible to the host, respectively.
	HOST_COHERENT = int_cast(vk::MemoryPropertyFlagBits::eHostCoherent),
	// specifies that the memory type only allows device access to the memory, and allows protected queue operations to access the memory.
	PROTECTED = int_cast(vk::MemoryPropertyFlagBits::eProtected),
};

_CREATE_ENUM_(CommandBufferUsageFlags, vk::CommandBufferUsageFlagBits){
	ONE_TIME_SUBMIT = int_cast(vk::CommandBufferUsageFlagBits::eOneTimeSubmit),
	SIMULATENOUS_USE = int_cast(vk::CommandBufferUsageFlagBits::eSimultaneousUse),
	RENDERPASS_CONTINUE = int_cast(vk::CommandBufferUsageFlagBits::eRenderPassContinue),

};

_CREATE_ENUM_(DynamicStates, vk::DynamicState){
	VIEWPORT = int_cast(vk::DynamicState::eViewport),
	SCISSOR = int_cast(vk::DynamicState::eScissor),
};

_CREATE_ENUM_(PrimitiveTopologies, vk::PrimitiveTopology){
	TRIANGLE = int_cast(vk::PrimitiveTopology::eTriangleList),
	LINE = int_cast(vk::PrimitiveTopology::eLineList)};

_CREATE_ENUM_(PolygonModes, vk::PolygonMode){
	FILL = int_cast(vk::PolygonMode::eFill),
	LINE = int_cast(vk::PolygonMode::eLine),
	POINT = int_cast(vk::PolygonMode::ePoint)

};

_CREATE_ENUM_(CullModes, vk::CullModeFlagBits){
	BACK = int_cast(vk::CullModeFlagBits::eBack),
	FRONT = int_cast(vk::CullModeFlagBits::eFront),
	FRONT_AND_BACK = int_cast(vk::CullModeFlagBits::eFrontAndBack),
	NONE = int_cast(vk::CullModeFlagBits::eNone),
};

_CREATE_ENUM_(ColorComponents, vk::ColorComponentFlagBits){
	RED = int_cast(vk::ColorComponentFlagBits::eR),
	BLUE = int_cast(vk::ColorComponentFlagBits::eB),
	GREEN = int_cast(vk::ColorComponentFlagBits::eG),
	ALPHA = int_cast(vk::ColorComponentFlagBits::eA),
};

_CREATE_ENUM_(DescriptorTypes, vk::DescriptorType){
	UNIFORM_BUFFER = int_cast(vk::DescriptorType::eUniformBuffer),
	STORAGE_BUFFER = int_cast(vk::DescriptorType::eStorageBuffer),
	COMBINED_IMAGE_SAMPLER = int_cast(vk::DescriptorType::eCombinedImageSampler),
	UNIFORM_TEXEL_BUFFER = int_cast(vk::DescriptorType::eUniformTexelBuffer),

};
_CREATE_ENUM_(DescriptorBindingFlags, vk::DescriptorBindingFlagBits){
	/*indicates that if descriptors in this binding are updated between when the descriptor
	set is bound in a command buffer and when that command buffer is submitted to a queue,
	then the submission will use the most recently set descriptors for this binding and the
	updates do not invalidate the command buffer.*/
	UPDATE_AFTER_BIND = int_cast(vk::DescriptorBindingFlagBits::eUpdateAfterBind),

	/*indicates that descriptors in this binding that are not dynamically used need not
	contain valid descriptors at the time the descriptors are consumed. A descriptor is
	dynamically used if any shader invocation executes an instruction that performs any
	memory access using the descriptor. If a descriptor is not dynamically used, any
	resource referenced by the descriptor is not considered to be referenced during
	command execution*/
	PARTIALLY_BOUND = int_cast(vk::DescriptorBindingFlagBits::ePartiallyBound),

};

_CREATE_ENUM_(SamplerAddressModes, vk::SamplerAddressMode){
	CLAMP_TO_BORDER = int_cast(vk::SamplerAddressMode::eClampToBorder),
	CLAMP_TO_EDGE = int_cast(vk::SamplerAddressMode::eClampToEdge),
	MIRROR_CLAMP_TO_EDGE = int_cast(vk::SamplerAddressMode::eMirrorClampToEdge),
	MIRRORED_REPEAT = int_cast(vk::SamplerAddressMode::eMirroredRepeat),
	REPEAT = int_cast(vk::SamplerAddressMode::eRepeat),
};

_CREATE_ENUM_(BorderColor, vk::BorderColor){
	FLOAT_OPAQUE_BLACK = int_cast(vk::BorderColor::eFloatOpaqueBlack),
	FLOAT_OPAQUE_WHITE = int_cast(vk::BorderColor::eFloatOpaqueWhite),
	FLOAT_TRANSPARENT_BLACK = int_cast(vk::BorderColor::eFloatTransparentBlack),
	INT_OPAQUE_BLACK = int_cast(vk::BorderColor::eIntOpaqueBlack),
	INT_OPAQUE_WHITE = int_cast(vk::BorderColor::eIntOpaqueWhite),
	INT_TRANSPARENT_BLACK = int_cast(vk::BorderColor::eIntTransparentBlack),
};
_CREATE_ENUM_(CompareOp, vk::CompareOp){
	ALWAYS = int_cast(vk::CompareOp::eAlways),
	EQUAL = int_cast(vk::CompareOp::eEqual),
	GREATER = int_cast(vk::CompareOp::eGreater),
	GREATER_EQUAL = int_cast(vk::CompareOp::eGreaterOrEqual),
	LESS = int_cast(vk::CompareOp::eLess),
	LESS_EQUAL = int_cast(vk::CompareOp::eLessOrEqual),
	NEVER = int_cast(vk::CompareOp::eNever),
	NOT_EQUAL = int_cast(vk::CompareOp::eNotEqual),

};
_CREATE_ENUM_(Filter, vk::Filter){
	LINEAR = int_cast(vk::Filter::eLinear),
	NEAREST = int_cast(vk::Filter::eNearest),
	CUBIC_EXT = int_cast(vk::Filter::eCubicEXT),

};
_CREATE_ENUM_(MipMapMode, vk::SamplerMipmapMode){
	LINEAR = int_cast(vk::SamplerMipmapMode::eLinear),
	NEAREST = int_cast(vk::SamplerMipmapMode::eNearest),

};
_CREATE_ENUM_(ColorSpace, vk::ColorSpaceKHR){
	SRGB_NON_LINEAR = int_cast(vk::ColorSpaceKHR::eSrgbNonlinear),
};

_CREATE_ENUM_(PresentModes, vk::PresentModeKHR){
	FIFO = int_cast(vk::PresentModeKHR::eFifo),
	FIFO_RELAXED = int_cast(vk::PresentModeKHR::eFifoRelaxed),
	IMMEDIATE = int_cast(vk::PresentModeKHR::eImmediate),
	MAILBOX = int_cast(vk::PresentModeKHR::eMailbox),
	SHARED_CONTINUOUS_REFRESH = int_cast(vk::PresentModeKHR::eSharedContinuousRefresh),
	SHARED_DEMAND_REFRESH = int_cast(vk::PresentModeKHR::eSharedDemandRefresh),
};
_CREATE_ENUM_(SurfaceTransformFlags, vk::SurfaceTransformFlagBitsKHR){
	HORIZONTAL_MIRROR = int_cast(vk::SurfaceTransformFlagBitsKHR::eHorizontalMirror),
	HORIZONTAL_MIRROR_ROTATE180 = int_cast(vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180),
	HORIZONTAL_MIRROR_ROTATE270 = int_cast(vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270),
	HORIZONTAL_MIRROR_ROTATE90 = int_cast(vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90),
	IDENTITY = int_cast(vk::SurfaceTransformFlagBitsKHR::eIdentity),
	INHERIT = int_cast(vk::SurfaceTransformFlagBitsKHR::eInherit),
	ROTATE180 = int_cast(vk::SurfaceTransformFlagBitsKHR::eRotate180),
	ROTATE270 = int_cast(vk::SurfaceTransformFlagBitsKHR::eRotate270),
	ROTATE90 = int_cast(vk::SurfaceTransformFlagBitsKHR::eRotate90),
};

_CREATE_ENUM_(CompositeAlphaModes, vk::CompositeAlphaFlagBitsKHR){
	INHERIT = int_cast(vk::CompositeAlphaFlagBitsKHR::eInherit),
	OPAQUE = int_cast(vk::CompositeAlphaFlagBitsKHR::eOpaque),
	PRE_MULTIPLIED = int_cast(vk::CompositeAlphaFlagBitsKHR::ePreMultiplied),
	POST_MULTIPLIED = int_cast(vk::CompositeAlphaFlagBitsKHR::ePostMultiplied),
};
_CREATE_ENUM_(PipelineBindPoint, vk::PipelineBindPoint){
	GRAPHICS = int_cast(vk::PipelineBindPoint::eGraphics),
	COMPUTE = int_cast(vk::PipelineBindPoint::eGraphics),
	RAY_TRACING = int_cast(vk::PipelineBindPoint::eRayTracingKHR),
};


#endif