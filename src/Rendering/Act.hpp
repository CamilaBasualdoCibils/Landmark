#pragma once
#include <pch.h>
#include <VK/Rendering/RenderPass.h>
#include "structs.h"
#include "Canvas.hpp"
class Frame;
class Film;
class Act {

    std::optional<RenderPass> rp;
    act_properties properties;
public:
    Act(const act_properties& _prop);
    ~Act();
    RenderPass& getRenderPass()  {return rp.value();}
    const act_properties& GetProperties() const {return properties;}

};