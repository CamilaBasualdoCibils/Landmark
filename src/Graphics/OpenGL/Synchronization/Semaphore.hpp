#pragma once
#include "Graphics/Interop/Interop.hpp"
#include <pch.h>

namespace GL
{
class Semaphore : public InteropReceiver
{

    GLuint Handle;

  public:
    Semaphore();
    ~Semaphore();
    void ImportMemory(InteropTransaction transaction) override;

    operator GLuint() const {return Handle;}
    operator GLuint() {return Handle;}
};
} // namespace GL