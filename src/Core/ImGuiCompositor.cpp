#include "ImGuiCompositor.hpp"

#include <Graphics/Compositor/CompositeContext.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/ImageCommands.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/PipelineCommands.hpp>
Graphics::CompositeLayerExecute ImGuiCompositor::OnRender(const Graphics::CompositeContext &Context)
{
    vk::RenderingAttachmentInfo colorAttachment;

    colorAttachment.imageView = *GetOutput()->VK().GetImageView();
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
    // vkCmdMngr->Push<VK::ClearColorImageCommand>(GetOutput(), VK::ImageLayouts::eGeneral, vec4{0, 0, 0, 1});
    // vkCmdMngr->Push(VK::BlitImageCommand(Context.previousLayers.back()->GetOutput(), GetOutput(),
    //                                     VK::ImageLayouts::eGeneral, VK::ImageLayouts::eGeneral, VK::Filter::eNearest,
    //                                     ivec3(0), ivec3(0), ivec3(GetResolution(), 0)));

    GenericAfterCmdMngr->Push(
        VK::TransitionImageLayoutCommand(GetOutput(), VK::ImageLayouts::eGeneral, VK::ImageLayouts::eGeneral,
                                         VK::PipelineStageFlags::eAllCommands, VK::PipelineStageFlags::eAllCommands));
    GenericAfterCmdMngr->PopGroupLabel();
}
