#include "CommandManager.hpp"
#include "SynchronizationCommands.hpp"
#include <GL/gl.h>

GL::CommandManager::CommandManager()
{
}
void GL::CommandManager::Submit()
{
    for (const auto &cmd : GetCommands())
    {
        cmd->Execute(nullptr);
    }
    glFlush();
}
void GL::CommandManager::SignalSemaphore(GPURef<Graphics::Semaphore> semaphore)
{
    Push<GL::SignalSemaphore>(semaphore);
}
void GL::CommandManager::WaitSemaphore(GPURef<Graphics::Semaphore> semaphore)
{
    Push<GL::WaitSemaphore>(semaphore);
}
[[nodiscard]] GPURef<Graphics::Semaphore> GL::CommandManager::SignalSemaphore()
{
    GPURef<Graphics::Semaphore> Semaphore = GPURef<Graphics::Semaphore>::MakeRef();
    Push<GL::SignalSemaphore>(Semaphore);
    return Semaphore;
}
[[nodiscard]] GPURef<Graphics::Semaphore> GL::CommandManager::WaitSemaphore()
{
    GPURef<Graphics::Semaphore> Semaphore = GPURef<Graphics::Semaphore>::MakeRef();
    Push<GL::WaitSemaphore>(Semaphore);
    return Semaphore;
}
void GL::CommandManager::Clear()
{
    Commands.clear();
}
void GL::CommandManager::InsertLabel(const std::string &Label, vec4 Color)
{

}
void GL::CommandManager::PushGroupLabel(const std::string &Group, vec4 Color)
{

}
void GL::CommandManager::PopGroupLabel()
{

}