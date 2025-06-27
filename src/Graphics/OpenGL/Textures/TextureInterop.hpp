#pragma once
#include "Graphics/Interop/Interop.hpp"
#include "Graphics/OpenGL/MemoryObject.hpp"
#include "ITexture.hpp"
#include <pch.h>

namespace GL {

    using TextureInteropProperties = ITextureProperties;
    class TextureInterop:public ITexture, public InteropReceiver
    {

        MemoryObject memObject;
        public:
        TextureInterop(const TextureInteropProperties& Properties);
        void ImportMemory(InteropTransaction transaction) override;
    };
}