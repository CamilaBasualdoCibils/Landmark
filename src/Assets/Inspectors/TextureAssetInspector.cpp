#include <Assets/Descriptors/TextureAssetDescriptor.hpp>
#include "TextureAssetInspector.hpp"

#include <VK/Buffers/Buffer.h>
#include <VK/Operations/SingleUseCommandBuffer.h>
#include <VK/Devices/LogicalDevice.h>
#include <Core/App.h>
#include <Assets/AssetManager.hpp>
#include <Assets/LoadedAsset/LoadedTextureAsset.hpp>
void TextureAssetInspector::RebuildDescriptorSet()
{
    if (imgui_ds.has_value())
    {
        ImGui_ImplVulkan_RemoveTexture(*imgui_ds);
        imgui_ds.reset();
    }

    if (sampler.has_value())
    {
        sampler->Destroy();
        sampler.reset();
    }
    Sampler::SamplerProperties samp_prop;
    samp_prop.magFilter = Filter::NEAREST;
    samp_prop.minFilter = Filter::NEAREST;
    sampler.emplace(samp_prop);

    imgui_ds.emplace();
    imgui_ds = ImGui_ImplVulkan_AddTexture((vk::Sampler)*sampler, (vk::ImageView)vkimage->GetImageView(), (VkImageLayout)(vk::ImageLayout)ImageLayouts::SHADER_READ_ONLY);
}
void TextureAssetInspector::SaveChanges()
{
}
void TextureAssetInspector::SetAsset(const Asset &_asset)
{
    AssetInspector::SetAsset(_asset);
    SetTitleBar(GetAsset().GetName());

    if (vkimage.has_value())
    {

        vkimage->Destroy();
        vkimage.reset();
    }

    Asset asset = GetAsset();
    original_descriptor = asset.GetDescriptor().GetTextureDescription();
    modified_descriptor = original_descriptor;
    modified = false;

    vkimage = asset.Load<LoadedTextureAsset>()->LoadNewGpuImage();
    // imgui_ds = ImGui_ImplVulkan_AddTexture((vk::Sampler)vkimage->GetSampler(), (vk::ImageView)vkimage->GetImageView(), VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    RebuildDescriptorSet();
}

void TextureAssetInspector::DrawWindowContents()
{

    if (ImGui::BeginChild("Topbar", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders))
    {
        if (!modified)
            ImGui::BeginDisabled();
        if (ImGui::Button("Save", ImVec2(50, 50)))
            SaveChanges();
        if (!modified)
            ImGui::EndDisabled();
        }
        ImGui::EndChild();

    const int min_image_display_x_size = ImGui::GetContentRegionAvail().x * 0.5f;
    const int max_image_display_x_size = ImGui::GetContentRegionAvail().x * 0.8f;
    ImGui::SetNextWindowSizeConstraints(ImVec2{min_image_display_x_size, -1}, ImVec2{max_image_display_x_size, -1});
    if (ImGui::BeginChild("image_display", ImVec2{max_image_display_x_size, -1}, ImGuiChildFlags_ResizeX))
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
            ImPlot::PlotImage("Image", (ImTextureID)(VkDescriptorSet)*imgui_ds, ImPlotPoint{0, 0}, ImPlotPoint{image_size.x, image_size.y});
            ImPlot::GetPlotDrawList()->AddRect(ImPlot::PlotToPixels(ImVec2{0, 0}), ImPlot::PlotToPixels(ImVec2{image_size.x, image_size.y}), ImGui::ColorConvertFloat4ToU32(ImVec4{1, 1, 1, 1}));
            ImPlot::EndPlot();
            // ImGui::Image((ImTextureID)(VkDescriptorSet)imgui_ds, ImGui::GetContentRegionAvail());
        }
    }
    ImGui::EndChild();
    ImGui::SameLine();
    // ImGui::SetNextWindowSizeConstraints(ImVec2{0,-1},ImVec2{max_image_display_x_size,-1});

    if (ImGui::BeginChild("parameters", ImVec2(0, 0), ImGuiChildFlags_Borders))
    {

        modified |= modified_descriptor.DisplayProperties();
    }
    ImGui::EndChild();
}

TextureAssetInspector::~TextureAssetInspector()
{
    if (sampler.has_value())
    {
        sampler->Destroy();
        sampler.reset();
    }
}
