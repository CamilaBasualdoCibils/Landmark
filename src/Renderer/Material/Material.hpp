#pragma once
#include "IO/File.h"
#include "IO/IO.h"
#include <VK/Rendering/Pipeline.h>
#include "MaterialLayout.hpp"

struct MaterialProperties
{
    std::string Source;
};
class Material
{
    std::optional<Pipeline> raster_pipe;

public:
    Material(const MaterialLayout &layout, const MaterialProperties &properties);
};
