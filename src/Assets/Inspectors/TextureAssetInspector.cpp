#include <Assets/Descriptors/TextureAssetDescriptor.hpp>
#include "TextureAssetInspector.hpp"

#include <VK/Buffers/Buffer.h>
#include <VK/Operations/SingleUseCommandBuffer.h>
#include <VK/Devices/LogicalDevice.h>
#include <Core/App.h>
void TextureAssetInspector::SetAsset(const Asset &_asset)
{
    AssetInspector::SetAsset(_asset);
    SetName(GetAsset().GetName());
    if (vkimage.has_value())
    {

        vkimage->Destroy();
        vkimage.reset();
    }
    if (imgui_ds != 0)
    {
        ImGui_ImplVulkan_RemoveTexture(imgui_ds);
        imgui_ds = vk::DescriptorSet{};
    }
    Asset asset = GetAsset();
    if (asset.GetType() != AssetType::eTEXTURE)
        throw "oops";
    const AssetDescriptor &desc = asset.GetDescriptor();
    const TextureAssetDescriptor texture_info = desc.GetTextureDescription();
    CombinedImageSampler::CISProperties prop;
    prop.image.dimensions = uvec3{texture_info.info.dimensions.x, texture_info.info.dimensions.y, 1};
    prop.image.format = Format::RGBA8SRGB;
    prop.image.usage = ImageUsage::SAMPLED | ImageUsage::TRANSFER_DST;
    // prop.image.memory_properties |= MemoryProperties::HOST_VISIBLE;
    // prop.image.;
    vkimage.emplace(prop);

    Buffer::BufferProperties staging_buffer_prop;
    staging_buffer_prop.size = vkimage->GetImage().GetMemorySize();
    staging_buffer_prop.memory_properties = {MemoryProperties::HOST_VISIBLE, MemoryProperties::HOST_COHERENT, MemoryProperties::DEVICE_LOCAL};
    staging_buffer_prop.usage_flags = BufferUsage::TRANSFER_SRC;
    Buffer staging_buffer(staging_buffer_prop);
    int x, y, channels;
    void *image_data = stb::stbi_load(GetAsset().GetPath().c_str(), &x, &y, &channels, 4);
    void *mapped = staging_buffer.MapMemory();
    std::memcpy(mapped, image_data, staging_buffer_prop.size);
    staging_buffer.UnmapMemory();
    stb::stbi_image_free(image_data);
    {

        CommandPool pool(App::GetInstance()->GetMainDevice()->GetGraphicsQueue());
        SingleUseCommandBuffer dispatch(pool);
        /*
        vk::ImageMemoryBarrier barrier;
        barrier.oldLayout = vk::ImageLayout::eUndefined;
        barrier.newLayout = vk::ImageLayout::eTransferDstOptimal;
        barrier.srcQueueFamilyIndex = vk::QueueFamilyIgnored;
        barrier.dstQueueFamilyIndex = vk::QueueFamilyIgnored;
        barrier.image = vkimage->GetImage();
        barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = vk::AccessFlagBits::eNone;
        barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
        dispatch->pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, vk::DependencyFlags{}, {}, {}, barrier);
       */
        vkimage->GetImage().TransferLayout(dispatch, ImageLayouts::UNDEFINED, ImageLayouts::TRANSFER_DST);

        /*vk::BufferImageCopy copy;
        copy.bufferOffset = 0;
        copy.bufferRowLength = 0;
        copy.bufferImageHeight = 0;
        copy.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
        copy.imageSubresource.mipLevel = 0;
        copy.imageSubresource.baseArrayLayer = 0;
        copy.imageSubresource.layerCount = 1;
        copy.imageOffset = vk::Offset3D{0, 0, 0};
        copy.imageExtent = vk::Extent3D{texture_info.info.dimensions.x, texture_info.info.dimensions.y, 1};
        dispatch->copyBufferToImage(staging_buffer, vkimage->GetImage(),
                                    (vk::ImageLayout)ImageLayouts::TRANSFER_DST, {copy});*/
        vkimage->GetImage().CopyFromBuffer(dispatch, staging_buffer);
        vkimage->GetImage().TransferLayout(dispatch, ImageLayouts::TRANSFER_DST, ImageLayouts::SHADER_READ_ONLY);

        /*
vk::ImageMemoryBarrier barrier2;
barrier2.oldLayout = vk::ImageLayout::eTransferDstOptimal;
barrier2.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
barrier2.srcQueueFamilyIndex = vk::QueueFamilyIgnored;
barrier2.dstQueueFamilyIndex = vk::QueueFamilyIgnored;
barrier2.image = vkimage->GetImage();
barrier2.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
barrier2.subresourceRange.baseMipLevel = 0;
barrier2.subresourceRange.levelCount = 1;
barrier2.subresourceRange.baseArrayLayer = 0;
barrier2.subresourceRange.layerCount = 1;
barrier2.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
barrier2.dstAccessMask = vk::AccessFlagBits::eShaderRead;
dispatch->pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlags{}, {}, {}, barrier2);
*/
    }
    staging_buffer.Destroy();
    imgui_ds = ImGui_ImplVulkan_AddTexture((vk::Sampler)vkimage->GetSampler(), (vk::ImageView)vkimage->GetImageView(), VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void TextureAssetInspector::DrawContents()
{
    ImGui::BeginChild("Topbar", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders);

    ImGui::BeginDisabled();
    ImGui::Button("Save");
    ImGui::EndDisabled();
    ImGui::EndChild();
    const int min_image_display_x_size = ImGui::GetContentRegionAvail().x * 0.5f;
    const int max_image_display_x_size = ImGui::GetContentRegionAvail().x * 0.8f;
    ImGui::SetNextWindowSizeConstraints(ImVec2{min_image_display_x_size, -1}, ImVec2{max_image_display_x_size, -1});
    ImGui::BeginChild("image_display", ImVec2{max_image_display_x_size, -1}, ImGuiChildFlags_ResizeX);
    {
        // std::cerr << ImGui::GetWindowSize().x << " " << max_image_display_x_size << std::endl;
        // if (ImGui::GetWindowSize().x > max_image_display_x_size)
        //     ImGui::SetWindowSize(ImVec2{max_image_display_x_size, -1});

        if (ImPlot::BeginPlot("image plot", ImVec2(-1, -1), ImPlotFlags_NoFrame | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoMouseText | ImPlotFlags_NoTitle | ImPlotFlags_Equal))
        {
            const ivec2 image_size = vkimage->GetImage().GetProperties().dimensions;
            //,ImVec2{-1,-1},ImPlotFlags_NoFrame|ImPlotFlags_NoBoxSelect|ImPlotFlags_NoLegend||ImPlotFlags_NoMenus|ImPlotFlags_NoMouseText |ImPlotFlags_NoTitle);
            ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoGridLines);
            ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoGridLines);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, -image_size.y * 0.5f, image_size.y * 1.5f);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, -image_size.y * 0.5f, image_size.y * 1.5f);
            ImPlot::SetupAxisZoomConstraints(ImAxis_X1, 1, image_size.x * 2);
            ImPlot::SetupAxisZoomConstraints(ImAxis_Y1, 1, image_size.y * 2);

            // ImPlot::SetupAxes(nullptr,nullptr,ImPlotAxisFlags_NoHighlight|,ImPlotAxisFlags_NoHighlight);
            ImPlot::PlotImage("Image", (ImTextureID)(VkDescriptorSet)imgui_ds, ImPlotPoint{0, 0}, ImPlotPoint{image_size.x, image_size.y});
            ImPlot::GetPlotDrawList()->AddRect(ImPlot::PlotToPixels(ImVec2{0, 0}), ImPlot::PlotToPixels(ImVec2{image_size.x, image_size.y}), ImGui::ColorConvertFloat4ToU32(ImVec4{1, 1, 1, 1}));
            ImPlot::EndPlot();
            // ImGui::Image((ImTextureID)(VkDescriptorSet)imgui_ds, ImGui::GetContentRegionAvail());
        }
    }
    ImGui::EndChild();
    ImGui::SameLine();
    // ImGui::SetNextWindowSizeConstraints(ImVec2{0,-1},ImVec2{max_image_display_x_size,-1});

    ImGui::BeginChild("parameters", ImVec2(0, 0), ImGuiChildFlags_Borders);
    TextureAssetDescriptor desc = GetAsset().GetDescriptor().GetTextureDescription();
    desc.DisplayProperties();
    ImGui::EndChild();
}
