#pragma once
#include <pch.h>

#include "Pass.hpp"
namespace Render
{
namespace Composite
{
class Pipeline;
struct PipelineSettings
{
    ivec2 BaseResolution = {1000, 1000};
};
struct PipelineProperties
{
PipelineSettings settings;
    Layout layout;
    std::vector<std::shared_ptr<Pipeline>> Dependencies;
};
/**
 * @brief This defines the context of a Rendering operation. This is the final output.
 *
 */
class Pipeline : public Pass
{
  PipelineSettings settings;

  public:
    Pipeline(std::string_view Name,const PipelineProperties &Properties);

    virtual void Compose(PipelinePayload &Payload);
    
    PipelineSettings GetEntrySettings() const{ return settings;}
    void SetEntrySettings(const PipelineSettings& newSettings,bool rebuild = true);
    private:
    //void Compose(Payload &Payload) override;
};
} // namespace Composite

} // namespace Render
