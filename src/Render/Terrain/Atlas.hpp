#pragma once

#include "AtlasDebugViewport.hpp"
#include <Editor/Editor.hpp>
#include <Types/SpatialTrees/Octree.hpp>
#include <pch.h>
const uint32 VoxelsPerMeter = 16;
const uint32 ChunksSizeMeters = 16;
struct ChunkData
{
};
constexpr uint32 IntLog2(uint32 x)
{
    uint32_t r = 0;
    while (x > 1)
    {
        x >>= 1;
        ++r;
    }
    return r;
}
constexpr uint32 CalcDepth(uint32 VoxelsPMeter, uint32 MetersPChunk)
{
    return IntLog2(MetersPChunk * VoxelsPMeter);
};
using ChunkOctree = Octree<CalcDepth(VoxelsPerMeter, ChunksSizeMeters), ChunkData>;
class Atlas : public Singleton<Atlas>
{

  public:
    Atlas()
    {
        std::shared_ptr<AtlasDebugViewport> debugViewport = Editor::Editor::Get()
                                                                .GetMainToolGroup()
                                                                .GetOrPushToolBar(Editor::Editor::Get().Tool_ToolBar)
                                                                ->PushObject<AtlasDebugViewport>();
        debugViewport->SetOpen(true);
    }
};