#pragma once
#include <pch.h>
#include "DeferredRenderPass.hpp"
#include "misc/Singleton.hpp"
class Renderer : public Singleton<Renderer>
{
    std::weak_ptr<Render::DeferredRenderPass> mainDeferredPass;
    std::shared_ptr<Render::Composite::Pipeline> RenderPipeline;

    Render::Composite::Layout DeferredRenderingLayout;
public:
Renderer();
auto GetPipeline() const {return RenderPipeline;}
};