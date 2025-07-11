#include "DearImGui.hpp"

#include "Graphics/GraphicsEngine.hpp"
#include "Graphics/Vulkan/Enums.hpp"
#include "Graphics/Vulkan/Rendering/RenderPass.hpp"
#include <Editor/Editor.hpp>
#include <Editor/EditorTemplates.hpp>
#include <Editor/EditorToolItem.hpp>
#include <Graphics/Compositor/CompositeContext.hpp>
#include <Graphics/Compositor/Compositor.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/ImageCommands.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/PipelineCommands.hpp>
#include <Graphics/Vulkan/Instance.hpp>
#include <misc/Conversions.hpp>
class ImGuiDemoTool : Editor::ToolItem
{
    bool Open = false;

    void DrawTool() override
    {
        if (Open)
            ImGui::ShowDemoWindow(&Open);
    }
    void DrawHandle() override
    {
        Editor::MenuItem(GetLabel().c_str(), &Open);
    }

  public:
    ImGuiDemoTool() : Editor::ToolItem("ImGui Demo")
    {
    }
};
class ImplotDemoTool : Editor::ToolItem
{
    bool Open = false;

    void DrawTool() override
    {
        if (Open)
            ImPlot::ShowDemoWindow(&Open);
    }
    void DrawHandle() override
    {
        Editor::MenuItem(GetLabel().c_str(), &Open);
    }

  public:
    ImplotDemoTool() : Editor::ToolItem("Implot Demo")
    {
    }
};
class ImPlotDemoTool
{
};
DearImGui::DearImGui(const DearImGuiProperties &Properties)
{

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
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

    io.FontAllowUserScaling = true;
    ImGuiStyle &style = ImGui::GetStyle();

    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;

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
    init_info.UseDynamicRendering = true;
    init_info.PipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
    VkFormat format = (VkFormat)vk::Format::eR8G8B8A8Srgb;
    init_info.PipelineRenderingCreateInfo.pColorAttachmentFormats = &format;
    init_info.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
    ImGui_ImplVulkan_Init(&init_info);
    {
        // Graphics::SingleUseCommandBuffer sucm;
        ImGui_ImplVulkan_CreateFontsTexture();
    }
    GetCompositeLayer();
    Editor::Editor::Get().GetMainToolGroup().GetOrPushToolBar("View")->PushObject<ImGuiDemoTool>();
    Editor::Editor::Get().GetMainToolGroup().GetOrPushToolBar("View")->PushObject<ImplotDemoTool>();
    // PushEditorTools();
}
void DearImGui::BeginFrame()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void DearImGui::EndFrame()
{
    ImGuiIO &io = ImGui::GetIO();
    ImGui::EndFrame();
    ImGui::Render();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

std::shared_ptr<ImGuiCompositor> DearImGui::GetCompositeLayer()
{
    if (!CompositeLayer)
        CompositeLayer = GraphicsEngine::Get().GetMainWindow()->GetCompositeGroup()->AddLayer<ImGuiCompositor>();

    return CompositeLayer;
}

Graphics::CompositeLayerExecute ImGuiCompositor::OnRender(const Graphics::CompositeContext &Context)
{
    vk::RenderingAttachmentInfo colorAttachment;

    colorAttachment.imageView = *GetAttachments().at("Main")->VK().GetImageView();
    colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.clearValue = vk::ClearColorValue{0, 0, 0, 0};

    vk::RenderingInfo renderingInfo;
    renderingInfo.renderArea = vk::Rect2D{vk::Offset2D{0, 0}, GlmToVkExtent(GetResolution())};
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &colorAttachment;
    ImGuiCmdManager->Clear();
    auto CmdBuffer = ImGuiCmdManager->GetManualCommandBuffer();
    CmdBuffer->Begin();
    CmdBuffer->GetHandle().beginRendering(renderingInfo);
    ImDrawData *DrawData = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(DrawData, (vk::CommandBuffer) * (CmdBuffer));
    CmdBuffer->GetHandle().endRendering();
    CmdBuffer->End();
    static Graphics::CompositeLayerExecute Execute{
        .CommandManagers = {GenericBeforeCmdMngr, ImGuiCmdManager, GenericAfterCmdMngr}};
    return Execute;
}

ImGuiCompositor::ImGuiCompositor(std::shared_ptr<Graphics::CompositeGroup> Parent)
    : ICompositeLayer(Parent, Graphics::CompositeLayerProperties{.Name = "ImGui Compositor"})
{
    GenericBeforeCmdMngr->PushGroupLabel("ImGuiComposite");

    GenericAfterCmdMngr->Push(
        VK::TransitionImageLayoutCommand(GetAttachments().at("Main"), VK::ImageLayouts::eGeneral, VK::ImageLayouts::eGeneral,
                                         VK::PipelineStageFlags::eAllCommands, VK::PipelineStageFlags::eAllCommands));
    GenericAfterCmdMngr->PopGroupLabel();
}
