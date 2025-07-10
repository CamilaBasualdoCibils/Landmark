#pragma once
#include "Graphics/OpenGL/Rendering/RenderTarget.hpp"
#include <Graphics/ICommandManager.hpp>
#include <pch.h>

namespace GL
{
struct Command : Graphics::ICommand<void *>
{
};
struct LambdaCommand : Command
{
    std::function<void()> function;
    LambdaCommand(std::function<void()> func) : function(func)
    {
    }
    std::string GetName() override
    {
        return "LambdaCommand";
    }
    void Execute(void *) override
    {
        function();
    }
};
class CommandManager : public Graphics::ICommandManager
{
    std::deque<std::unique_ptr<Command>> Commands;

  public:
    CommandManager();
    void SignalSemaphore(GPURef<Graphics::Semaphore> semaphore) override;
    [[nodiscard]] GPURef<Graphics::Semaphore> SignalSemaphore() override;
    void WaitSemaphore(GPURef<Graphics::Semaphore> semaphore) override;
    [[nodiscard]] GPURef<Graphics::Semaphore> WaitSemaphore() override;
    void Submit();
    void BeginRendering(GPURef<Graphics::RenderTarget> rt) override
    {
        Push<LambdaCommand>([rt]() { rt->GL().BindDraw(); });
    }
    void EndRendering() override
    {
        Push<LambdaCommand>([]() { RenderTarget::UnBind(); });
    }
    const decltype(Commands) &GetCommands() const
    {
        return Commands;
    }
    template <typename T, typename... ARGS> void Push(ARGS... args)
    {
        // Check that the API commands given correspond to the API of this command buffer
        const bool ValidCommand = (std::is_base_of_v<GL::Command, T>);
        LASSERT(ValidCommand, "Invalid Command");

        Commands.push_back(std::make_unique<T>(std::forward<ARGS>(args)...));
    };
    void Clear() override;
    void InsertLabel(const std::string &Label, vec4 Color = {1, 1, 1, 1}) override;
    void PushGroupLabel(const std::string &Group, vec4 Color = {1, 1, 1, 1}) override;
    void PopGroupLabel() override;
};
} // namespace GL