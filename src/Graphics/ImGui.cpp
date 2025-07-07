#include "ImGui.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "Graphics/Vulkan/Images/ImageEnums.hpp"
#include "Graphics/Vulkan/Rendering/RenderPass.hpp"
#include "pch.h"
#include <Graphics/Vulkan/Instance.hpp>
#include <algorithm>
#include <vulkan/vulkan_enums.hpp>

DearImGui::DearImGui(const DearImGuiProperties &Properties)
{

    RenderPass.Make(
        VK::RenderPassProperties{.Attachments = {VK::RenderPassAttachment{.Format = VK::ImageFormats::eBGRA8_UNorm}},
                                 .Subpasses = {VK::RenderPassSubpass{
                                     .SubpassAttachments = {VK::RenderPassSubpassAttachment{
                                         .AttachmentID = 0, .IdealLayout = VK::ImageLayouts::eColorAttachment}}}}});

    const auto Window = GraphicsEngine::Get().GetMainWindow();
    const auto GPU = GraphicsEngine::Get().GetMainGPU()->VK();
    VkDescriptorPoolSize pool_sizes[] = {{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
                                         {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                                         {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
                                         {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
                                         {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
                                         {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
                                         {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
                                         {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
                                         {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                                         {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                                         {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000;
    pool_info.poolSizeCount = std::size(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;
    const auto CreatePoolResult = GPU->LogicalDevice()->GetHandle().createDescriptorPool(pool_info);
    LASSERT(CreatePoolResult.result == vk::Result::eSuccess, "concha");
    // VK_CHECK(vkCreateDescriptorPool(_device, &pool_info, nullptr, &imguiPool));
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    //(void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

    io.FontAllowUserScaling = true;
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    const auto capabilties = GPU->LogicalDevice()->GetSurfaceCapabilities(Window->GetVulkanSurface());
    ImPlot::CreateContext();
    ImGui_ImplGlfw_InitForVulkan(Window->GetGLFWHandle(), true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = static_cast<vk::Instance>(VK::Instance::Get());
    init_info.PhysicalDevice = GPU->LogicalDevice()->GetVkPhysicalDeviceHandle();
    init_info.Device = GPU->LogicalDevice()->GetHandle();
    init_info.Queue = static_cast<vk::Queue>(GPU->GraphicsQueue()->GetHandle());
    init_info.DescriptorPool = CreatePoolResult.value;
    init_info.MinImageCount = capabilties.minImageCount;
    init_info.ImageCount = std::max(capabilties.maxImageCount, std::max(capabilties.minImageCount + 1, uint32_t(3)));
    init_info.ImageCount = std::clamp(uint32_t(3), capabilties.minImageCount, capabilties.maxImageCount);
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.RenderPass = RenderPass->GetHandle();

    ImGui_ImplVulkan_Init(&init_info);
    {
        // Graphics::SingleUseCommandBuffer sucm;
        ImGui_ImplVulkan_CreateFontsTexture();
    }

    // PushEditorTools();
}