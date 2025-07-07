#include "Semaphore.hpp"
#include "Graphics/GraphicsEngine.hpp"
#include "pch.h"
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>


VK::Semaphore::Semaphore()
{
    const vk::ExternalSemaphoreHandleTypeFlagBits HandleType = vk::ExternalSemaphoreHandleTypeFlagBits::eOpaqueFd;
    vk::ExportSemaphoreCreateInfo ExportInfo;
    ExportInfo.setHandleTypes(HandleType);

    vk::SemaphoreTypeCreateInfo TypeInfo;
    TypeInfo.semaphoreType = vk::SemaphoreType::eBinary;
    TypeInfo.initialValue = 0;
    vk::SemaphoreCreateInfo CreateInfo;
    CreateInfo.setPNext(TypeInfo);
    TypeInfo.setPNext(&ExportInfo);

    std::shared_ptr<VK::Device> DeviceHandle = GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice();
    const auto CreateResult = DeviceHandle->GetHandle().createSemaphore(CreateInfo);
    LASSERT(CreateResult.result == vk::Result::eSuccess, "Shit");
    Handle = CreateResult.value;

    vk::SemaphoreGetFdInfoKHR getFdInfo;
    getFdInfo.setHandleType(HandleType);
    getFdInfo.semaphore = Handle;

    
    const vk::Result FDResult = (vk::Result)DeviceHandle->GetSemaphoreFdKHR(getFdInfo,&fdHandle);

    LASSERT(FDResult == vk::Result::eSuccess, "FD fao;ire");
}
VK::Semaphore::~Semaphore()
{
    GraphicsEngine::Get().GetMainGPU()->VK()->LogicalDevice()->GetHandle().destroy(Handle);
}