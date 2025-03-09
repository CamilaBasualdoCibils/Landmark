#pragma once
#include <VK/Rendering/Framebuffer.h>
class Canvas {
public:
Canvas() {}
    virtual Framebuffer& GetFramebuffer() = 0; 
    virtual ivec2 GetFramebufferSize() const =0;
};