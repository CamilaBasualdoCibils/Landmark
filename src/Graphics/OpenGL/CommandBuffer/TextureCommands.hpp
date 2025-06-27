#pragma once
#include "Graphics/ICommandBuffer.hpp"
#include "Graphics/OpenGL/CommandBuffer/CommandBuffer.hpp"
#include "Graphics/Texture.hpp"
#include <pch.h>

namespace GL {

    struct BindTexture : Command
    {
        GPURef<Graphics::Texture> Texture;
        GLuint Unit;
        BindTexture(GLuint _Unit,GPURef<Graphics::Texture> _Texture):Texture(_Texture),Unit(_Unit){}
        void Execute(void*) override
        {
            glBindTextureUnit(Unit,Texture->GL());
        }
        std::string GetName() override
        {
            return "BindTextureUnit";
        }
    };
}