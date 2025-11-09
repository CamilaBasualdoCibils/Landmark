#include "Compositor.hpp"
#include "Graphics/GraphicsEngine.hpp"
std::shared_ptr<Render::Composite::Pipeline> Render::Compositor::MakeBasePipeline(std::string_view Name,
    const Composite::PipelineProperties &properties)
{
    auto ptr = std::make_shared<Composite::Pipeline>(Name,properties);
    Pipelines.push_back(std::weak_ptr<Composite::Pipeline>(ptr));
    return ptr;
}

void Render::Compositor::Compose()
{
    Render::Composite::PipelinePayload payload;
    payload.cmdManager = GPURef<VK::CommandManager>::MakeRef();
    for (auto it = Pipelines.begin();it!=Pipelines.end();)
    {
        if (it->expired())
        {
            Pipelines.erase(it);
            
            continue;
        }

        auto pipeline = it->lock();
        if (!pipeline->IsBuilt()) pipeline->Build();
        pipeline->Compose(payload);
        ++it;
    }

    GraphicsEngine::Get().Push({payload.cmdManager});
}
