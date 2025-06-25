#include "Schedule.hpp"
#include "Lens.h"
#include "Film.hpp"
#include "Act.hpp"
#include "Stage.h"
Schedule::Schedule(const schedule_properties &_prop) : properties(_prop)
{
    LASSERT(_prop.rendering_queue, "Invalid Rendering Queue");
    LASSERT(_prop.presention_queue, "Invalid Presentation Queue");
    cmd_pool.emplace(*_prop.rendering_queue);
    cmd_buffer.emplace(*cmd_pool);
    schedule_finished_semaphore.emplace();
    schedule_finished_fence.emplace();
}

Schedule &Schedule::Clear()
{
    cmd_buffer->Reset();
    return *this;
}

Schedule &Schedule::Execute()
{

    cmd_buffer->Submit(Semaphores_before_execute, {*schedule_finished_semaphore}, {*schedule_finished_fence});
    Semaphores_before_execute.clear();
    return *this;
}

CommandBuffer &Schedule::GetReadyCmdBuffer()
{
    if (!cmd_buffer->HasBegun())
        cmd_buffer->Begin();
    return *cmd_buffer;
}

Schedule &Schedule::Begin()
{
    cmd_buffer->Begin();
    return *this;
}

Schedule &Schedule::End()
{
    cmd_buffer->End();
    return *this;
}

Schedule &Schedule::BeginAct(Canvas &target, Act &act)
{
    LASSERT(!act_ongoing, "BeginAct called without ending previous");
    act_ongoing = true;
    current_canvas = &target;
    const uvec2 fbo_size = target.GetFramebufferSize();
    cmd_buffer->BeginRenderPass(act.getRenderPass(), target.GetFramebuffer(), {{0, 0}, {fbo_size.x, fbo_size.y}});
    return *this;
}

Schedule &Schedule::EndAct()
{
    LASSERT(act_ongoing, "EndAct called but no act is ongoing");
    cmd_buffer->EndRenderPass();
    act_ongoing = false;
    current_canvas = nullptr;
    return *this;
}

Schedule &Schedule::SetViewport(Viewport bounds)
{
    cmd_buffer->SetViewport(bounds);
    return *this;
}

Schedule &Schedule::SetScissor(IRect2D bounds)
{
    cmd_buffer->SetScissor(bounds);
    return *this;
}

Schedule &Schedule::PrepareNextFrame(Film &proj, Act &act)
{
    Semaphores_before_execute.push_back(proj.NextFrame());
    while (proj.isOutOfDate())
    {
        proj.Reconstruct(proj.GetProperties(), act);
        Semaphores_before_execute.back() = proj.NextFrame();
    }
    return *this;
}

Schedule &Schedule::Present(Film &proj)
{
    if (proj.isOutOfDate())
        return *this;
    
    proj.Present(properties.presention_queue, {*schedule_finished_semaphore});
    return *this;
}

Schedule &Schedule::WaitUntilIdle()
{
    schedule_finished_fence->WaitAndReset();
    schedule_finished_fence->GetvkDevice().waitIdle();
    return *this;
}

Schedule &Schedule::Capture(Stage &stg, const schedule_capture_stage_properties &prop)
{
    stg.Draw(*cmd_buffer);
    return *this;
}

Schedule &Schedule::UseLens(Lens &lens)
{
    cmd_buffer->BindPipeline(lens.getPipeline(), PipelineBindPoint::GRAPHICS);
    return *this;
}

Schedule &Schedule::FullViewport()
{
    const auto size = current_canvas->GetFramebufferSize();
    Viewport vp;
    vp.size = size;
    SetViewport(vp);
    return *this;
}

Schedule &Schedule::FullScissor()
{
    const auto size = current_canvas->GetFramebufferSize();
    IRect2D rect;
    rect.min = {0, 0};
    rect.max = size;
    SetScissor(rect);
    return *this;
}

Schedule &Schedule::PrepareStage(Stage &stg)
{
    LASSERT(!act_ongoing, "Cannot prepare a stage while an act is ongoing");
    stg.TransferData(*cmd_buffer);
    return *this;
}
