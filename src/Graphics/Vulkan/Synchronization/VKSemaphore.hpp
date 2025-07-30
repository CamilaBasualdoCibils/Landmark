#pragma once
#include "Graphics/Interop/Interop.hpp"
#include <pch.h>
namespace VK
{

class Semaphore :InteropGiver
{
    vk::Semaphore Handle;
    FDHandle fdHandle;

  public:
    Semaphore();
    ~Semaphore();

    InteropTransaction ExportMemory() const override {return {fdHandle,0};}
    vk::Semaphore GetHandle() const {return Handle;}
};
} // namespace VK