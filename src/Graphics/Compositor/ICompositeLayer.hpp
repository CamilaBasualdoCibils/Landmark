#pragma once
#include <pch.h>
#include <Graphics/GPURef.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/Synchronization.hpp>
#include <Graphics/ICommandManager.hpp>
namespace Graphics
{
    class CompositeGroup;
    struct CompositeContext;
    struct CompositeLayerProperties
    {
        std::string Name;
        std::optional<uvec2> ResolutionOverride;
    };
    struct CompositeLayerExecute
    {
        std::vector<std::shared_ptr<Graphics::ICommandManager>> CommandManagers;
        //GPURef<Graphics::Semaphore> DoneSemaphore;
    };
    class ICompositeLayer
    {
        GPURef<Graphics::Texture> Texture;
        std::shared_ptr<CompositeGroup> Parent = nullptr;
        CompositeLayerProperties Properties;
        virtual CompositeLayerExecute OnRender(const CompositeContext& Context) = 0;
        public:
        ICompositeLayer(std::shared_ptr<CompositeGroup> Parent,const CompositeLayerProperties& Properties);
        CompositeLayerExecute Render(const CompositeContext& Context)
        {
            return OnRender(Context);
        }
        void SetParent(std::shared_ptr<CompositeGroup> NewParent) {Parent = NewParent;}
        std::shared_ptr<CompositeGroup> GetParent() const {return Parent;}
        const CompositeLayerProperties& GetProperties() const {return Properties;}
        uvec2 GetResolution() const;
        public:
        virtual GPURef<Graphics::Texture> GetOutput() {return Texture;}
        virtual const GPURef<Graphics::Texture> GetOutput() const {return Texture;}

    };
} // namespace Graphics
