#pragma once
#include <pch.h>
#include "ICompositeLayer.hpp"
#include <Graphics/Vulkan/Commands/CommandManager/CommandManager.hpp>
#include <Graphics/OpenGL/Commands/CommandManager.hpp>
namespace Graphics
{
    struct CompositeContext
    {
        std::vector<std::shared_ptr<ICompositeLayer>> previousLayers;
        //std::shared_ptr<VK::CommandManager> VKCommandManager;
        //std::shared_ptr<GL::CommandManager> GLCommandManager;
    };
} // namespace Graphics
