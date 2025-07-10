#pragma once
#include "Graphics/OpenGL/Commands/CommandManager.hpp"
#include "Graphics/Synchronization.hpp"
#include <pch.h>
namespace GL
{

struct SignalSemaphore : Command
{
    GPURef<Graphics::Semaphore> semaphore;
    SignalSemaphore(GPURef<Graphics::Semaphore> _semaphore) :semaphore(_semaphore) {}

    void Execute(void *context) override{
        glSignalSemaphoreEXT(semaphore->GL(),0,nullptr,0,nullptr,nullptr);
    }
    std::string GetName() override {return "SignalSemaphore";}
};
struct WaitSemaphore : Command
{
    GPURef<Graphics::Semaphore> semaphore;
    WaitSemaphore(GPURef<Graphics::Semaphore> _semaphore) :semaphore(_semaphore) {}

    void Execute(void *context) override{
        glWaitSemaphoreEXT(semaphore->GL(),0,nullptr,0,nullptr,nullptr);
    }
    std::string GetName() override {return "WaitSemaphore";}
};
} // namespace GL