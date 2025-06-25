//
// Created by camila on 10/29/24.
//

#ifndef FRAME_H
#define FRAME_H
#include "Canvas.hpp"
#include "structs.h"
#include <VK/Rendering/Framebuffer.h>
class Act;
class Frame:public Canvas {

    frame_properties properties;
    std::optional<Framebuffer> framebuffer;
    
static inline Log logger = Log("frame");
    public:
    Frame(const frame_properties& o,Act& act);
    ~Frame();
    Framebuffer& GetFramebuffer() override {return framebuffer.value();}
    ivec2 GetFramebufferSize() const override  {return framebuffer->GetSize();}
    const frame_properties& getProperties() const {return properties;}
};



#endif //FRAME_H
