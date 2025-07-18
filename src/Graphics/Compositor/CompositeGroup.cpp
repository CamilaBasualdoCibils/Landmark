#include "CompositeGroup.hpp"

Graphics::CompositeGroup::CompositeGroup(const CompositeLayerProperties &Properties):ICompositeLayer(nullptr,Properties)
{
    
}

Graphics::CompositeLayerExecute Graphics::CompositeGroup::OnRender(const Graphics::CompositeContext &OldContext)
{
    Graphics::CompositeLayerExecute Execute;
    Graphics::CompositeContext Context(OldContext);
    for (const auto& Layer : Layers)
    {
        CompositeLayerExecute result = Layer->Render(Context);
        Execute.CommandManagers.insert(Execute.CommandManagers.end(),result.CommandManagers.begin(),result.CommandManagers.end());
        //LASSERT(!(result.CommandManagers.empty() ^!result.DoneSemaphore),"If commands provided then a Done semaphore must be provided as well");
        Context.previousLayers.push_back(Layer);
    }
    return Execute;
}
