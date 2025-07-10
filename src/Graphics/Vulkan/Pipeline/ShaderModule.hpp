#pragma once
#include <Graphics/GraphicsEngine.hpp>
#include <pch.h>

namespace VK
{
class ShaderModule
{
    vk::ShaderModule Handle;
    const std::vector<std::byte> Binary;

  public:
    ShaderModule(const std::vector<std::byte> &binary) : Binary(binary)
    {
        vk::ShaderModuleCreateInfo CreateInfo;
        CreateInfo.pCode = (uint32_t *)Binary.data();
        CreateInfo.codeSize = Binary.size();
        const auto CreateResult =
            GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().createShaderModule(CreateInfo);
        LASSERT(CreateResult.result == vk::Result::eSuccess, "Fuck");
        Handle = CreateResult.value;
    }
    ~ShaderModule()
    {
        GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().destroyShaderModule(Handle);
    }
    operator vk::ShaderModule() const
    {
        return Handle;
    }
};
} // namespace VK