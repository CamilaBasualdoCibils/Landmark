
#pragma once
#include "renderdoc_app.h"
#include <VK/Vulkan_Instance.h>
#include <Debug/Logging/Log.h>
#include <Core/Window.h>
class RenderDoc {
    static inline RENDERDOC_API_1_6_0* rdoc_api = nullptr;
    static inline Log log = Log("RenderDoc");
    public:
    static void Init();
    static void SetActiveWindow(Vulkan_Instance& device, LWindow& wndHandle);
    static void StartCapture();
    static void EndCapture();
};