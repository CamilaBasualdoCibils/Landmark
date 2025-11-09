#include "DearImGui.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "Graphics/Vulkan/VKInstance.hpp"
#include "Platform/Window/WindowManager.hpp"
#include "Render/Compositor/Compositor.hpp"
DearImGui::DearImGui()
{
    const auto Window = Platform::WindowManager::Get().GetMainWindow();
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
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

    // io.FontAllowUserScaling = true;
    ImGuiStyle &style = ImGui::GetStyle();

    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    const auto capabilties = GPU->LogicalDevice()->GetSurfaceCapabilities(Window->GetVulkanSurface());

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
    init_info.UseDynamicRendering = true;
    init_info.PipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
    VkFormat format = (VkFormat)(vk::Format)CurrentFormat;
    init_info.PipelineRenderingCreateInfo.pColorAttachmentFormats = &format;
    init_info.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
    init_info.ApiVersion = vk::makeApiVersion(0, 1, 4, 0);
    ImGui_ImplVulkan_Init(&init_info);
    {
        // Graphics::SingleUseCommandBuffer sucm;
        ImGui_ImplVulkan_CreateFontsTexture();
    }

    Render::Composite::Layout ImGuiLayout;
    Render::Composite::ColorAttachmentInfo ColorAttachment;
    ColorAttachment.format = CurrentFormat;
    ColorAttachment.Usage = {VK::ImageUsage::eColorAttachment, VK::ImageUsage::eTransferDst,
                             VK::ImageUsage::eTransferSrc};
    ColorAttachment.Name = "Color";

    ImGuiLayout.PushColorAttachment(ColorAttachment);
    ImGuiLayout.Build();

    Pipeline = Render::Compositor::Get().MakeBasePipeline("ImGuiPipeline", Render::Composite::PipelineProperties{
                                                                               .settings = ivec2{1920, 1080},
                                                                               .layout = ImGuiLayout,
                                                                           });
    Pipeline->Build();
}

void DearImGui::BeginFrame()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void DearImGui::UpdatePlatforms()
{
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void DearImGui::EndFrame()
{
    ImGui::EndFrame();
    ImGui::Render();
}
