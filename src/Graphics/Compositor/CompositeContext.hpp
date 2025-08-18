#pragma once
#include <pch.h>

#include <Graphics/Vulkan/Commands/CommandManager/VKCommandManager.hpp>
#include <Graphics/OpenGL/Commands/GLCommandManager.hpp>
#include "ICompositeLayer.hpp"
namespace Graphics
{
    struct CompositeContext
    {
        std::vector<std::shared_ptr<ICompositeLayer>> previousLayers;

    };
} // namespace Graphics
