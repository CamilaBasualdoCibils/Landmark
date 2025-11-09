#include "Pipeline.hpp"
#include "Pass.hpp"
Render::Composite::Pipeline::Pipeline(std::string_view Name, const PipelineProperties &Properties)
    : Pass(Name, PassProperties{.layout = Properties.layout}),settings(Properties.settings)
{
}

void Render::Composite::Pipeline::Compose(PipelinePayload &payload)
{
    Payload layerPayload;
    layerPayload.pipelinePayload = payload;
    Pass::Compose(layerPayload);
}

void Render::Composite::Pipeline::SetEntrySettings(const PipelineSettings &newSettings, bool rebuild)
{
    settings = newSettings;
    Build();
}
