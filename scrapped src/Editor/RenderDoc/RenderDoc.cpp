#include <pch.h>
#include "RenderDoc.h"
#include <dlfcn.h>
void RenderDoc::Init()
{
    if (void *mod = dlopen("./librenderdoc.so", RTLD_NOW ))
    {
        pRENDERDOC_GetAPI RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)dlsym(mod, "RENDERDOC_GetAPI");
        int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_6_0, (void **)&rdoc_api);
        if (ret != 1)
        {
            LASSERT(ret == 1, "Renderdoc failed to init");
            
        }
        
        log.Debug("Init");
       
    }
    else
    {
        const char *message = dlerror();
        log.Debug(std::string("Error loading RenderDoc: ") + message);
        LASSERT(false, "Failed to initialize RenderDoc");
    }
}

void RenderDoc::SetActiveWindow(Vulkan_Instance &device, LWindow &wndHandle)
{
     VkInstance vk_instance = (vk::Instance)device;
        LWindow::internal_window_handle wHandle = wndHandle.GetInternalHandle();
        rdoc_api->SetActiveWindow(RENDERDOC_DEVICEPOINTER_FROM_VKINSTANCE(vk_instance),&wndHandle);
}

void RenderDoc::StartCapture()
{
    rdoc_api->StartFrameCapture(nullptr, nullptr);
    log.Debug("Capture Start");
}

void RenderDoc::EndCapture()
{
    rdoc_api->EndFrameCapture(nullptr, nullptr);
    log.Debug("Capture End");
    rdoc_api->TriggerCapture();
}
