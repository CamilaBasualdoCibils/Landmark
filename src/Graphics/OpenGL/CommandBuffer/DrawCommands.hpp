#pragma once
#include "Graphics/ICommandBuffer.hpp"
#include <pch.h>
namespace GL
{
    struct DrawArrays : Command
    {
        DrawArrays(GLenum mode, GLint first, GLsizei count)
        {
            
        }

        void Execute(void *context) override{

        }
    }
    
}