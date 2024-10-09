#include <pch.h>
#include <Time/TimeTypes.h>
#include <G_API/VulkanInstance.h>
#include <Core/AppModule.h>
#include <Debug/Logging/LogKeeper2.h>

struct AppProperties
{
    std::string app_name;
    milliseconds log_flush_interval = milliseconds(100);
    bool default_modules = true;
};
class App
{

    std::string app_name = "default app name";
    std::optional<VulkanInstance> vk_instance;
    std::vector<std::unique_ptr<AppModule>> modules;
    // sort by injection then priority
    std::map<AppModule::EngineCallPoints, std::vector<AppModule::EngineCallInject>> module_call_injections;
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
    private:
    void CallInjections(AppModule::EngineCallPoints point);
    void RegisterInjection(const AppModule::EngineCallInject& inject);
    void SetupSelfInjections();
};