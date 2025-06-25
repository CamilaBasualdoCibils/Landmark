#pragma once
#include <pch.h>
#include <VK/Rendering/PipelineLayout.h>
struct MaterialLayoutProperties
{
};

class MaterialLayout
{
private:
    std::optional<PipelineLayout> raster_layout;
    /* data */
public:
    MaterialLayout(const MaterialLayoutProperties &prop);
    ~MaterialLayout();
};
