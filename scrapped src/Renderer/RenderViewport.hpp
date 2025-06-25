#pragma once
#include <pch.h>
#include <ECS/ECS.hpp>
#include <Transform/Transform.h>
#include <Renderer/Camera.h>
#include <Rendering/Act.hpp>
#include <Rendering/Frame.h>
#include <Rendering/Stage.h>

#include<Assets/Asset.hpp>
struct RenderViewportCreateInfo {
    uvec2 initial_size = {500,500};
    Format image_format = Format::BGRA_8_UNORM;

};
struct RenderViewport_ViewData {
    mat4 view,proj,viewproj;
};
class RenderViewport {
    //std::shared_ptr<CombinedImageSampler> image;
    std::shared_ptr<Act> act;
    std::shared_ptr<Frame> frame;

    RenderViewport_ViewData viewdata;
    bool enabled = true;
    uvec2 size;
    //Camera camera;
    //Transform transform;
    public:
    RenderViewport(const RenderViewportCreateInfo& info);
    void SetSize(uvec2 new_size);
    uvec2 GetSize() const {return size;}
    constexpr decltype(frame) GetFrame() {return frame;}
    constexpr decltype(act) GetAct() {return act;}

    void SetViewData(const RenderViewport_ViewData& vd) {viewdata = vd;}
   const decltype(viewdata)& GetViewData() const {return viewdata;}

};