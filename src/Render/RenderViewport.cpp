#include "RenderViewport.hpp"
#include "WorldRenderer.hpp"
void RenderViewport::DrawWindowContents()
{

    ImGui::Image((ImTextureID)WorldRenderer::Get().GetCompositeGroup()->GetAttachments().at("Color")->VK().GetImguiDescriptorSet(),
                 ImGui::GetContentRegionAvail());
}
