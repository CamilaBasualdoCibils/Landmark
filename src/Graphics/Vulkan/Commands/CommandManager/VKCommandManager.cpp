#include "VKCommandManager.hpp"
#include "VKDebugCommands.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "VKRenderingCommands.hpp"
#include "pch.h"
VK::CommandManager::CommandManager(const Flags<VK::CmdCapatiblities> &Capabilities) : Capabilities(Capabilities)
{

    CmdPool = GPURef<VK::CommandPool>::MakeRef(GraphicsEngine::Get().GetMainGPU()->VK()->GraphicsQueue());
}

void VK::CommandManager::MakePartition()
{

    Partitions.emplace_back();
    Partition &P = Partitions.back();
    P.CmdBuffer = GPURef<VK::CommandBuffer>::MakeRef(CmdPool);
}

VK::CommandManager::Partition::Partition()
{
}
VK::CommandManager::Partition::~Partition()
{
}
void VK::CommandManager::BeginRendering(GPURef<VK::RenderTarget> rt)
{
    Push<VK::BeginRenderingCommand>(rt);
}
void VK::CommandManager::EndRendering()
{
    Push<VK::EndRenderingCommand>();
}
void VK::CommandManager::Clear()
{
    Partitions.clear();
    PartitionsDirty = true;
}
void VK::CommandManager::InsertLabel(const std::string &Label, vec4 Color)
{
    Push(VK::InsertLabelCommand(Label, Color));
}
void VK::CommandManager::PushGroupLabel(const std::string &Group, vec4 Color)
{
    Push(VK::BeginGroupCommand(Group, Color));
}
void VK::CommandManager::PopGroupLabel()
{
    Push(VK::EndGroupCommand());
}
GPURef<VK::CommandBuffer> VK::CommandManager::GetManualCommandBuffer()
{

    // if it is manual and is recording give that one
    if (Partitions.empty() || !Partitions.back().ManualParition || !Partitions.back().CmdBuffer->IsRecording())
    {
        MakePartition();
        Partitions.back().ManualParition = true;
    }

    return Partitions.back().CmdBuffer;
}
void VK::CommandManager::Submit(GPURef<VK::Queue> queue)
{
    if (PartitionsDirty)
    {
        if (Partitions.back().commands.empty() && !Partitions.back().ManualParition)
            Partitions.pop_back();
        PartitionsDirty = false;
        for (auto &Partition : Partitions)
        {
            if (Partition.ManualParition)
            {
                continue;
            }

            Partition.CmdBuffer->Reset();

            Partition.CmdBuffer->Begin();
            for (const auto &Command : Partition.commands)
            {
                Command->Execute(Partition.CmdBuffer);
            }
            Partition.CmdBuffer->End();
        }
    }

    for (const auto &Partition : Partitions)
    {
        vk::SubmitInfo SubmitInfo;
        vk::CommandBuffer buffer[1] = {Partition.CmdBuffer->GetHandle()};
        SubmitInfo.setCommandBuffers(buffer);
        vk::Semaphore WaitSemaphoreHandle, SignalSemaphoreHandle;
        const vk::PipelineStageFlags WaitStageMask = vk::PipelineStageFlagBits::eAllCommands;
        ;
        SubmitInfo.pWaitDstStageMask = &WaitStageMask;
        if (Partition.WaitSemaphore)
        {
            WaitSemaphoreHandle = Partition.WaitSemaphore->VK().GetHandle();
            SubmitInfo.setWaitSemaphores({WaitSemaphoreHandle});
        }
        if (Partition.SignalSemaphore)
        {
            SignalSemaphoreHandle = Partition.SignalSemaphore->VK().GetHandle();
            SubmitInfo.setSignalSemaphores({SignalSemaphoreHandle});
        }

        const auto SubmitResult = queue->GetHandle().submit(
            SubmitInfo, Partition.CompletionFence ? Partition.CompletionFence->GetHandle() : VK_NULL_HANDLE);
    }
}