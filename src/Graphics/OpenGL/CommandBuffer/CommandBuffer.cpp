#include "CommandBuffer.hpp"
#include "Graphics/OpenGL/CommandBuffer/SynchronizationCommands.hpp"
#include <GL/gl.h>

GL::CommandBuffer::CommandBuffer()
{
    
}
void GL::CommandBuffer::Submit()
{
    for (const auto& cmd : GetCommands())
    {
        cmd->Execute(nullptr);
    }
    glFlush();
}
void GL::CommandBuffer::SignalSemaphore(GPURef<Graphics::Semaphore> semaphore)
{
    Push<GL::SignalSemaphore>(semaphore);
}void GL::CommandBuffer::WaitSemaphore(GPURef<Graphics::Semaphore> semaphore)
{
    Push<GL::WaitSemaphore>(semaphore);
}
void GL::CommandBuffer::Clear() {
    Commands.clear();
}