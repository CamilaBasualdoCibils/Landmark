#pragma once
#include <pch.h>
#include <type_traits>

using FDHandle = int;
struct InteropTransaction
{
    FDHandle fdHandle;
    size_t AllocationSize;
};
class InteropGiver
{
  public:
    virtual InteropTransaction ExportMemory() const = 0;
};

class InteropReceiver
{
    virtual void ImportMemory(InteropTransaction) = 0;
};
enum InteropTransactionType
{
    eVK2GL,
    eGL2VK,
    eNone
};
template <typename VulkanObject, typename OpenGLObject, InteropTransactionType TransactionType> class InteropObject
{

    std::optional<VulkanObject> vulkanObject;
    std::optional<OpenGLObject> openGLObject;

  protected:
    template <typename VulkanArg, typename GLArg> void Initialize(VulkanArg vulkanArg, GLArg glArg)
    {
        vulkanObject.emplace(vulkanArg);
        openGLObject.emplace(glArg);
        TransferMemory();
    }
    void Initialize()
    {
        vulkanObject.emplace();
        openGLObject.emplace();
        TransferMemory();
    }
    void TransferMemory()
    {
        if constexpr (TransactionType == InteropTransactionType::eVK2GL)
            openGLObject->ImportMemory(vulkanObject->ExportMemory()); // if vulkan gives, OpenGL receives
        else if constexpr (TransactionType == InteropTransactionType::eGL2VK)
            vulkanObject->ImportMemory(openGLObject->ExportMemory()); // if openGL gives, Vulkan receives
    }
    void CheckObjects() const
    {
        LASSERT(vulkanObject.has_value() && openGLObject.has_value(), "Object not initialized");
    }

  public:
    template <typename VulkanArg = void, typename GLArg = void,
              typename = std::enable_if_t<!std::is_same_v<VulkanArg, void> && !std::is_same_v<GLArg, void>>>
    InteropObject(VulkanArg &&vulkanArg, GLArg &&glArg)
    {
        Initialize(std::forward<VulkanArg>(vulkanArg), std::forward<GLArg>(glArg));
    }
    InteropObject()
    {
        if constexpr (std::is_default_constructible_v<VulkanObject> && std::is_default_constructible_v<OpenGLObject>)
        {
            Initialize();
        }
    }
    const VulkanObject &VK() const
    {
        CheckObjects();
        return *vulkanObject;
    }
    VulkanObject &VK()
    {
        CheckObjects();
        return *vulkanObject;
    }

    const OpenGLObject &GL() const
    {
        CheckObjects();
        return *openGLObject;
    }
    OpenGLObject &GL()
    {
        CheckObjects();
        return *openGLObject;
    }

    operator OpenGLObject()
    {
        CheckObjects();
        return *openGLObject;
    }
    operator OpenGLObject() const
    {
        CheckObjects();
        return *openGLObject;
    }
    operator VulkanObject()
    {
        CheckObjects();
        return *vulkanObject;
    }
    operator VulkanObject() const
    {
        CheckObjects();
        return *vulkanObject;
    }
};
