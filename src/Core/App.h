#pragma once
#include <pch.h>
#include <Time/TimeTypes.h>
#include <VK/Vulkan_Instance.h>
#include <Core/AppModule.h>
#include <Debug/Logging/LogKeeper2.h>
#include <Core/Window.h>
#include <VK/Devices/MainDevice.h>
#include <Renderer/Renderer.hpp>

//#define RENDERDOC_CAPTURE
struct AppProperties
{
    std::string app_name;
    milliseconds log_flush_interval = milliseconds(100);
    bool default_modules = true;
};
class App
{
    static inline App *instance = nullptr;
    std::string app_name = "default app name";
    std::shared_ptr<LWindow> main_window;
    // std::optional<LWindow> main_window;
    std::shared_ptr<Vulkan_Instance> main_vk_instance;
    std::shared_ptr<MainDevice> main_vk_device;
    std::vector<std::shared_ptr<AppModule>> modules;
    std::map<AppModule::EngineCallPoints, std::vector<AppModule::EngineCallInject>> module_call_injections; // sorted by priority
    bool should_terminate = false;
    Log logger = Log("App");

public:
    App();
    virtual void Run(const AppProperties &_properties);
    template <typename T,typename ... Args> //,std::enable_if<std::is_base_of_v<AppModule,T>>>
    std::shared_ptr<T> AttachModule(Args&&... args)
    {
        std::shared_ptr<T> t = std::make_shared<T>(std::forward<Args>(args)...);
        std::shared_ptr<AppModule> base = t;
        modules.push_back(base);
        const auto injections = (*modules.back()).GetInjections();
        for (const auto &inject : injections)
        {
            RegisterInjection(inject);
        }
        return std::dynamic_pointer_cast<T>(modules.back());
    }
    template <typename T> //,std::enable_if<std::is_base_of_v<AppModule,T>>>
    std::shared_ptr<T> GetModule()
    {
        for (auto &module : modules)
        {
            auto dyn_mod = std::dynamic_pointer_cast<T>(module);
             if (dyn_mod)
            {
                return dyn_mod;
            }
        }
        return nullptr;
    }
    static App *GetInstance()
    {
        LASSERT(instance, "Instance doesnt exist?");
        return instance;
    }
    decltype(main_vk_device) GetMainDevice()
    {
        LASSERT(main_vk_device.get(), "Device does not exist");
        return main_vk_device;
    }
    decltype(main_vk_instance) GetMainInstance() {
        LASSERT(main_vk_instance.get(), "Instance does not exist");
        return main_vk_instance;
    }
    decltype(main_window) GetMainWindow() {
        LASSERT(main_window.get(), "Window Does not exist");
        return main_window;
    }

private:
    void CallInjections(AppModule::EngineCallPoints point);
    void RegisterInjection(const AppModule::EngineCallInject &inject);
    void SetupSelfInjections();
    void InitializeVulkan();
    void CleanUpVulkan();
    void SetupMainVkDevice();
};