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
   original_descriptor.descriptor_data = modified_tex_descriptor;
   Asset asset = GetAsset();
   AssetManager::GetInstance()->OverwriteAssetDescriptor(asset,original_descriptor);
   modified = false;
   //AssetManager::ReloadAsset(GetAsset());
}
void TextureAssetInspector::DrawTextureView()
{
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

        modified |= modified_tex_descriptor.DisplayProperties();
    }
    ImGui::EndChild();
}
void TextureAssetInspector::DrawSpriteSheetView()
{
    if (ImGui::BeginChild("TopBar", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY))
    {
        {
            int grid_rate_mode = (int)sprite_inspector_vars.grid_rate_mode;
            const char *grid_mode_labels[] = {"None", "Pixel", "Texcoord"};
            if (ImGui::Combo("Grid mode", &grid_rate_mode, grid_mode_labels, 3))
            {
                sprite_inspector_vars.grid_rate_mode = (SpriteInspectorVars::GridRates)grid_rate_mode;
            }
            ImGui::SameLine();
            if (sprite_inspector_vars.grid_rate_mode != SpriteInspectorVars::GridRates::ePIXEL)
            {
                const bool disabled = sprite_inspector_vars.grid_rate_mode != SpriteInspectorVars::GridRates::eTexcoord;
                if (disabled)
                    ImGui::BeginDisabled();
                ImGui::DragFloat2("Grid rate", &sprite_inspector_vars.grid_rate.x, 0.01, 0, 1);
                if (disabled)
                    ImGui::EndDisabled();
            }
            else if (sprite_inspector_vars.grid_rate_mode == SpriteInspectorVars::GridRates::ePIXEL)
            {
                ivec2 to_int = sprite_inspector_vars.grid_rate;
                if (ImGui::DragInt2("Grid rate", &to_int.x))
                {
                    to_int = glm::max(to_int, ivec2(0));
                    sprite_inspector_vars.grid_rate = to_int;
                }
            }
        }

        ImGui::SameLine();
        {
            int snap_mode = (int)sprite_inspector_vars.snapping;
            const char *snap_mode_labels[] = {"None", "Pixel", "Grid"};
            if (ImGui::Combo("Snap mode", &snap_mode, snap_mode_labels, 3))
            {
                sprite_inspector_vars.snapping = (SpriteInspectorVars::SnapMode)snap_mode;
            }
        }

        // ImGui::SameLine();
        // ImGui::BeginCombo()
    }
    ImGui::EndChild();

    auto &sprites = modified_tex_descriptor.parameters.spritesheet.sprites;
    const int min_left_side_x_size = ImGui::GetContentRegionAvail().x * 0.5f;
    const int max_left_side_x_size = ImGui::GetContentRegionAvail().x * 0.8f;
    ImGui::SetNextWindowSizeConstraints(ImVec2(min_left_side_x_size, -1), ImVec2(max_left_side_x_size, 0));
    if (ImGui::BeginChild("Left side", ImVec2(max_left_side_x_size, -1), ImGuiChildFlags_ResizeX))
    {
        if (ImPlot::BeginPlot("image plot", ImVec2(-1, -1), ImPlotFlags_NoFrame | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoMouseText | ImPlotFlags_NoTitle | ImPlotFlags_Equal))
        {
            const ivec2 image_size = vkimage->GetImage().GetProperties().dimensions;
            //,ImVec2{-1,-1},ImPlotFlags_NoFrame|ImPlotFlags_NoBoxSelect|ImPlotFlags_NoLegend||ImPlotFlags_NoMenus|ImPlotFlags_NoMouseText |ImPlotFlags_NoTitle);
            //ImPlot::SetupAxisLinks()
            ImPlot::SetupAxis(ImAxis_X1, "Texcoord", ImPlotAxisFlags_NoGridLines);
            ImPlot::SetupAxis(ImAxis_Y1, "Texcoord", ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_Invert);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, -0.5, 1.5f);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, -0.5f, 1.5f);
            ImPlot::SetupAxisZoomConstraints(ImAxis_X1, 0.001, 2);
            ImPlot::SetupAxisZoomConstraints(ImAxis_Y1, 0.001, 2);
            ImPlot::PlotImage("Image", (ImTextureID)(VkDescriptorSet)*imgui_ds, ImPlotPoint{0, 1}, ImPlotPoint{1, 0});
            ImPlot::GetPlotDrawList()->AddRect(ImPlot::PlotToPixels(ImVec2{0, 0}), ImPlot::PlotToPixels(ImVec2{1, 1}), ImGui::ColorConvertFloat4ToU32(ImVec4{1, 1, 1, 1}));
            if (sprite_inspector_vars.currently_selected_sprite_ID != -1)
            {
                auto &sprite = sprites[sprite_inspector_vars.currently_selected_sprite_ID];
                dvec2 min = sprite.uv_min, max = sprite.uv_max;
                bool held = false;
                if (ImPlot::DragRect(0, &min.x, &min.y, &max.x, &max.y, ImVec4(1, 1, 1, 1),0,nullptr,nullptr,&held))
                {
                    modified = true;
                    if (sprite_inspector_vars.snapping == SpriteInspectorVars::SnapMode::ePIXEL)
                    {
                        min = Screen2UV(glm::round(Uv2Screen(min)));
                        max = Screen2UV(glm::round(Uv2Screen(max)));
                    }
                    sprite.uv_max = max;
                    sprite.uv_min = min;
                }
            }
            // ImPlot::SetupAxes(nullptr,nullptr,ImPlotAxisFlags_NoHighlight|,ImPlotAxisFlags_NoHighlight);
           
            ImPlot::EndPlot();
            // ImGui::Image((ImTextureID)(VkDescriptorSet)imgui_ds, ImGui::GetContentRegionAvail());
        }
    }
    ImGui::EndChild();
    ImGui::SameLine();

    if (ImGui::BeginChild("Right side", ImVec2(-1, -1)))
    {
        ImGui::SetNextWindowSize(ImGui::GetContentRegionAvail() * ImVec2(1, 0.6), ImGuiCond_Once);
        ImGui::SetNextWindowSizeConstraints(ImGui::GetContentRegionAvail() * ImVec2(1, 0.2), ImGui::GetContentRegionAvail() * ImVec2(1, 0.8));
        if (ImGui::BeginChild("SpriteList", ImVec2(0, 0), ImGuiChildFlags_ResizeY))
        {

            const int image_x_size = 30;
            if (ImGui::SmallButton("+"))
            {
                TextureAssetParameters::SpriteInfo spr;
                spr.name = "default name";
                spr.uv_max = {1, 1};
                spr.uv_min = {0, 0};
                sprites.push_back(spr);
                modified = true;
            }
            for (int i = 0; i < sprites.size(); i++)
            {
                TextureAssetParameters::SpriteInfo &sprite = sprites[i];
                const bool selected = i == sprite_inspector_vars.currently_selected_sprite_ID;
                if (ImGui::BeginChild(i + 1, ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY))
                {

                    if (selected)
                    {
                        ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive)));
                    }

                    else if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
                    {
                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                        {
                            sprite_inspector_vars.currently_selected_sprite_ID = i;
                        }
                        else
                            ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered)));
                    }

                    if (!selected)
                        ImGui::BeginDisabled();
                    /*
                {
                    const ImVec2 start_cursor = ImGui::GetCursorPos();
                    ImGui::SetNextItemAllowOverlap();
                    ImGui::Selectable("##selectable", &selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap, ImGui::GetContentRegionAvail());
                    ImGui::SetCursorPos(start_cursor);
                    }*/

                    if (ImGui::BeginChild("image", ImVec2(image_x_size, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY))
                    {
                        ImVec2 min = {sprite.uv_min.x, sprite.uv_min.y}, max = {sprite.uv_max.x, sprite.uv_max.y};
                        ImGui::Image((ImTextureID)(VkDescriptorSet)*imgui_ds, ImVec2(image_x_size, image_x_size), min, max);
                    }
                    ImGui::EndChild();
                    ImGui::SameLine();
                    if (ImGui::BeginChild("info", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY))
                    {
                        if (ImGui::InputText("Name", &sprite.name))
                        {
                            modified = true;
                        }
                        if (ImGui::DragFloat2("uv min", &sprite.uv_min.x, 0.01f, 0.0f, 1.0f))
                        {
                            sprite.uv_max = glm::max(sprite.uv_max, sprite.uv_min);
                            modified = true;
                        }
                        if (ImGui::DragFloat2("uv max", &sprite.uv_max.x, 0.01f, 0.0f, 1.0f))
                        {
                            sprite.uv_min = glm::min(sprite.uv_max, sprite.uv_min);
                            modified = true;
                        }
                    }
                    ImGui::EndChild();
                    if (!selected)
                        ImGui::EndDisabled();
                }

                ImGui::EndChild();
            }
        }
        ImGui::EndChild();

        if (ImGui::BeginChild("Sprite Visual", ImVec2(0, 0), ImGuiChildFlags_Borders))
        {

            if (sprite_inspector_vars.currently_selected_sprite_ID != -1)
            {
                
                const auto &spr = sprites[sprite_inspector_vars.currently_selected_sprite_ID];
                const vec2 spr_size = spr.uv_max - spr.uv_min;
                const float spr_aspect = spr_size.x/spr_size.y;
                const ImVec2 avail_space = ImGui::GetContentRegionAvail();
                const int max_size = std::min(avail_space.x,avail_space.y);
                 ImVec2 image_size = {max_size,max_size};
                const bool x_greater = spr_size.x > spr_size.y;
                if (x_greater) {
                    image_size.y *= spr_aspect;
                }
                else {
                    image_size.x /= spr_aspect;
                }
                ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2{avail_space.x/2 - image_size.x/2,0});

                ImVec2 min = {spr.uv_min.x, spr.uv_min.y}, max = {spr.uv_max.x, spr.uv_max.y};
                ImGui::Image((ImTextureID)(VkDescriptorSet)*imgui_ds,image_size, min, max);
            }
        }
        ImGui::EndChild();
    }
    ImGui::EndChild();
}
vec2 TextureAssetInspector::Uv2Screen(vec2 v)
{
    return v * (vec2)modified_tex_descriptor.info.dimensions;
}
vec2 TextureAssetInspector::Screen2UV(vec2 s)
{
    return s / (vec2)modified_tex_descriptor.info.dimensions;
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
    original_descriptor = asset.GetDescriptor();
    modified_tex_descriptor = original_descriptor.GetTextureDescription();
    modified = false;

    vkimage = asset.Load<LoadedTextureAsset>()->LoadNewGpuImage();
    // imgui_ds = ImGui_ImplVulkan_AddTexture((vk::Sampler)vkimage->GetSampler(), (vk::ImageView)vkimage->GetImageView(), VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    RebuildDescriptorSet();
}

void TextureAssetInspector::DrawWindowContents()
{

    if (ImGui::BeginChild("Topbar", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders))
    {

        const int Button_height = 50;
        if (ImGui::BeginChild("Save Section", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY))
        {
            bool mod = modified;
            if (!mod)
                ImGui::BeginDisabled();
            if (ImGui::Button("Save", ImVec2(Button_height, Button_height)))
                SaveChanges();
            if (!mod)
                ImGui::EndDisabled();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        if (ImGui::BeginChild("External Tools", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY))
        {
            const int text_size = ImGui::CalcTextSize("Button that has no more purpose").x;
            if (ImGui::Button("Button that has no more purpose", ImVec2(std::max(Button_height, text_size), Button_height)))
            {
            }
        }
        ImGui::EndChild();
    }
    ImGui::EndChild();

    if (ImGui::BeginTabBar("MyTabBar"))
    {
        if (ImGui::BeginTabItem("Texture"))
        {
            DrawTextureView();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Sprite Sheet"))
        {
            DrawSpriteSheetView();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Cucumber"))
        {

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

TextureAssetInspector::~TextureAssetInspector()
{
    if (sampler.has_value())
    {
        sampler->Destroy();
        sampler.reset();
    }
}
