#include <pch.h>
#include <Time/TimeTypes.h>
#include <VK/Vulkan_Instance.h>
#include <Core/AppModule.h>
#include <Debug/Logging/LogKeeper2.h>
#include <Core/Window.h>
#include <VK/Devices/MainDevice.h>
struct AppProperties
{
    std::string app_name;
    milliseconds log_flush_interval = milliseconds(100);
    bool default_modules = true;
};
class App
{
    static inline App* instance = nullptr;
    std::string app_name = "default app name";
    std::optional<Window> main_window;
    std::optional<Vulkan_Instance> main_vk_instance;
    std::optional<MainDevice> main_vk_device;
    std::vector<std::unique_ptr<AppModule>> modules;
    std::map<AppModule::EngineCallPoints, std::vector<AppModule::EngineCallInject>> module_call_injections; //sorted by priority
    bool should_terminate = false;
    LogKeeper2 *log_keeper = nullptr;
    Log logger = Log("App");

public:
    App();
    virtual void Run(const AppProperties &_properties);
    template <typename T> //,std::enable_if<std::is_base_of_v<AppModule,T>>>
    T *AttachModule()
    {
        
        modules.push_back(std::make_unique<T>());
        const auto injections = (*modules.back()).GetInjections();
        for (const auto &inject : injections)
        {
           RegisterInjection(inject);
        }

        return reinterpret_cast<T*>( &(*modules.back()));
    }
    static App* GetInstance() {LASSERT(instance,"Instance doesnt exist?"); return instance;}
    MainDevice* GetMainDevice() {return &main_vk_device.value();}
    private:
    void CallInjections(AppModule::EngineCallPoints point);
    void RegisterInjection(const AppModule::EngineCallInject& inject);
    void SetupSelfInjections();
    void InitializeVulkan();
    void CleanUpVulkan();
    void SetupMainVkDevice();
};