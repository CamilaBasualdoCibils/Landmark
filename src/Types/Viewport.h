#pragma once
#include <pch.h>
struct Viewport {
    vec2 pos = {0,0};
    vec2 size = {100,100};
    vec2 min_max_depth = {0,1};
    operator vk::Viewport() const {
        vk::Viewport vp;
        vp.x = pos.x;
        vp.y = pos.y;
        vp.width = size.x;
        vp.height = size.y;
        vp.minDepth = min_max_depth.x;
        vp.maxDepth = min_max_depth.y;
        return vp;
    }
};