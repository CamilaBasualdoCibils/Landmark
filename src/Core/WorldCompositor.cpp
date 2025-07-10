#include "WorldCompositor.hpp"
#include <Graphics/Vulkan/Commands/CommandManager/ImageCommands.hpp>
#include <Graphics/Vulkan/Commands/CommandManager/PipelineCommands.hpp>
Graphics::CompositeLayerExecute WorldCompositor::OnRender(const Graphics::CompositeContext &Context)
{

    static Graphics::CompositeLayerExecute Execute{.CommandManagers = {vkCmdMngr}};
    return Execute;
}

WorldCompositor::WorldCompositor(std::shared_ptr<Graphics::CompositeGroup> Parent)
    : ICompositeLayer(Parent, Graphics::CompositeLayerProperties{.Name = "WorldCompositor"})
{
    vkCmdMngr->Push<VK::ClearColorImageCommand>(GetOutput(), VK::ImageLayouts::eGeneral, vec4{0, 0, 0, 1});
}
