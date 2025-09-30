#pragma once
#include "CompositeLayer.hpp"
#include <pch.h>
namespace Render
{

    struct CompositePassProperties
    {
        
    };
class CompositePass
{

    std::vector<std::shared_ptr<CompositeLayer>> Layers;

  public:
    virtual void PreRender();
    virtual void PostRender();
};
} // namespace Render
