#include "WindowImGuiLayer.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "Platform/Window/Window.hpp"
#include "Platform/Window/WindowManager.hpp"
#include <Graphics/Vulkan/VKInstance.hpp>
#include "Editor/Editor.hpp"
#include "Editor/EditorLambdaItem.hpp"
#include "misc/Conversions.hpp"
Platform::WindowImGuiLayer::WindowImGuiLayer()
    : Layer("ImGui Layer")
{
}
bool Platform::WindowImGuiLayer::Build()
{
    const auto window = Platform::WindowManager::Get().GetMainWindow();
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
    ImPlot::CreateContext();
    ImPlot3D::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    //(void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

    // io.FontAllowUserScaling = true;
    ImGuiStyle &style = ImGui::GetStyle();

    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    const auto capabilties = GPU->LogicalDevice()->GetSurfaceCapabilities(window->GetVulkanSurface());
    return true;
    ImGui_ImplGlfw_InitForVulkan(window->GetGLFWHandle(), true);
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
    init_info.UseDynamicRendering = true;
    init_info.PipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
    VkFormat format = (VkFormat)(vk::Format)window->GetSwapchain()->GetImageFormat();
    init_info.PipelineRenderingCreateInfo.pColorAttachmentFormats = &format;
    init_info.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
    init_info.ApiVersion = vk::makeApiVersion(0, 1, 4, 0);
    ImGui_ImplVulkan_Init(&init_info);
    {
        // Graphics::SingleUseCommandBuffer sucm;
        ImGui_ImplVulkan_CreateFontsTexture();
    }

    static bool ImGuiStackTool = false;
    Editor::Editor::Get()
        .GetMainToolGroup()
        .GetOrPushToolBar("View")
        ->GetOrPushToolBar("ImGui")
        ->PushObject<Editor::LambdaItem>(
            0, "ImGui Stack Tool", [&]() { ImGui::Selectable("Stack Tool", &ImGuiStackTool); },
            [&]() {
                if (ImGuiStackTool)
                    ImGui::ShowIDStackToolWindow(&ImGuiStackTool);
            });
    static bool ImGuiMetrics = false;
    Editor::Editor::Get()
        .GetMainToolGroup()
        .GetOrPushToolBar("View")
        ->GetOrPushToolBar("ImGui")
        ->PushObject<Editor::LambdaItem>(
            0, "ImGui Metrics", [&]() { ImGui::Selectable("ImGui Metrics", &ImGuiMetrics); },
            [&]() {
                if (ImGuiMetrics)
                    ImGui::ShowMetricsWindow(&ImGuiMetrics);
            });
    static bool ImGuiDemoOpen = false;

    Editor::Editor::Get()
        .GetMainToolGroup()
        .GetOrPushToolBar("View")
        ->GetOrPushToolBar("ImGui")
        ->PushObject<Editor::LambdaItem>(
            1, "Imgui Demo", []() { ImGui::Selectable("ImGui Demo", &ImGuiDemoOpen); },
            [&]() {
                if (ImGuiDemoOpen)
                    ImGui::ShowDemoWindow(&ImGuiDemoOpen);
            });
    static bool ImPlotDemoOpen = false;
    Editor::Editor::Get()
        .GetMainToolGroup()
        .GetOrPushToolBar("View")
        ->GetOrPushToolBar("ImGui")
        ->PushObject<Editor::LambdaItem>(
            1, "ImPlot Demo", []() { ImGui::Selectable("ImPlot Demo", &ImPlotDemoOpen); },
            [&]() {
                if (ImPlotDemoOpen)
                    ImPlot::ShowDemoWindow(&ImPlotDemoOpen);
            });

    static bool ImPlot3DDemoOpen = false;
    Editor::Editor::Get()
        .GetMainToolGroup()
        .GetOrPushToolBar("View")
        ->GetOrPushToolBar("ImGui")
        ->PushObject<Editor::LambdaItem>(
            1, "ImPlot3D Demo", []() { ImGui::Selectable("ImPlot3D Demo", &ImPlot3DDemoOpen); },
            [&]() {
                if (ImPlot3DDemoOpen)
                    ImPlot3D::ShowDemoWindow(&ImPlot3DDemoOpen);
            });
    return true;
}

void Platform::WindowImGuiLayer::Compose(Render::Composite::Payload &payload)
{
    
    // vk::RenderingAttachmentInfo colorAttachment;
    //
    //colorAttachment.imageView = payload.ColorAttachments.front().Texture->GetImageView();
    //colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    //colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    //colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    //colorAttachment.clearValue = vk::ClearColorValue{0, 0, 0, 0};
//
    //
    //vk::RenderingInfo renderingInfo;
    //renderingInfo.renderArea = vk::Rect2D{vk::Offset2D{0, 0}, payload.};
    //renderingInfo.layerCount = 1;
    //renderingInfo.colorAttachmentCount = 1;
    //renderingInfo.pColorAttachments = &colorAttachment;
    payload.pipelinePayload.cmdManager->EndRendering();
    auto CmdBuffer = payload.pipelinePayload.cmdManager->GetManualCommandBuffer();
    
    CmdBuffer->Begin();
    const auto renderingInfo = payload.RenderTarget->AsRenderingInfo();
    CmdBuffer->GetHandle().beginRendering(renderingInfo->first);
    ImDrawData *DrawData = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(DrawData, (vk::CommandBuffer) * (CmdBuffer));
    CmdBuffer->GetHandle().endRendering();
    CmdBuffer->End();
    payload.pipelinePayload.cmdManager->BeginRendering(payload.RenderTarget);


}