#pragma once
#include <pch.h>
#include <vulkan/vulkan_handles.hpp>
#include "Graphics/Vulkan/Images/ImageEnums.hpp"
#include "Graphics/Vulkan/Pipeline/PipelineEnums.hpp"

namespace VK
{
enum class AttachmentLoadOp : std::underlying_type<vk::AttachmentLoadOp>::type
{
    eLoad = (size_t)vk::AttachmentLoadOp::eLoad,
    eClear = (size_t)vk::AttachmentLoadOp::eClear,
    eDontCare = (size_t)vk::AttachmentLoadOp::eDontCare,
    eNone = (size_t)vk::AttachmentLoadOp::eNone,
};
enum class AttachmentStoreOp : std::underlying_type<vk::AttachmentStoreOp>::type
{
    eDontCare = (size_t)vk::AttachmentStoreOp::eDontCare,
    eStore = (size_t)vk::AttachmentStoreOp::eStore,
    eNone = (size_t)vk::AttachmentStoreOp::eNone,
};
struct RenderPassAttachment
{
    AttachmentLoadOp LoadOp = AttachmentLoadOp::eLoad;
    AttachmentStoreOp StoreOp = AttachmentStoreOp::eStore;
    AttachmentLoadOp StencilLoadOp = AttachmentLoadOp::eLoad;
    AttachmentStoreOp StencilStoreOp = AttachmentStoreOp::eStore;
    ImageLayouts InitialLayout = ImageLayouts::eUndefined;
    ImageLayouts FinalLayout = ImageLayouts::ePresentSrc;
    ImageFormats Format = ImageFormats::eRGBA8_UNorm;
};
struct RenderPassSubpassAttachment
{
    uint32_t AttachmentID;
    ImageLayouts IdealLayout;
};
struct RenderPassSubpass
{
    PipelineBindPoint BindPoint = PipelineBindPoint::eGraphics;
    std::vector<RenderPassSubpassAttachment> SubpassAttachments;
};
struct RenderPassProperties
{
    std::vector<RenderPassAttachment> Attachments;
    std::vector<RenderPassSubpass> Subpasses;
};
class RenderPass
{
vk::RenderPass Handle;
RenderPassProperties Properties;
  public:
    RenderPass(const RenderPassProperties &Properties);
    operator vk::RenderPass() const {return Handle;}
    const RenderPassProperties& GetProperties() const {return Properties;}
    
};
} // namespace VK