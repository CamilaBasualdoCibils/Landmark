#pragma once
#include <pch.h>
#include "Graphics/Interop/Interop.hpp"

namespace GL {

    class MemoryObject : public InteropReceiver{
        GLuint Handle;
        size_t Size;
        public:
        MemoryObject();
        ~MemoryObject();

        void ImportMemory(InteropTransaction transaction) override;

        operator GLuint() const {return Handle;}
    };
}