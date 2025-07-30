#pragma once

#include "Graphics/GPURef.hpp"
#include "Graphics/OpenGL/CommandBuffer/GLCommandBuffer.hpp"
#include <pch.h>
namespace GL
{
    struct DrawArrays : Command
    {
        GLenum mode; GLint first; GLsizei count;
        DrawArrays(GLenum mode, GLint first, GLsizei count)
        {
            this->mode = mode;
            this->first = first;
            this->count = count;
        }
        std::string GetName() override
        {
            return "DrawArrays";
        }
        void Execute(void *context) override{
            glDrawArrays(mode, first,count);
        }
    };
    struct BeginRendering: Command
    {
        //BeginRendering(GPURef<typename T>)
    };
    
}