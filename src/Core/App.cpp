#include "App.h"
#include "Logo.h"
App::App()
{
}

void App::Run(const AppProperties &_properties)
{
    SetupSelfInjections();
    if (_properties.default_modules)
    {
        log_keeper = AttachModule<LogKeeper2>();
        // log_keeper->
    }

    CallInjections(AppModule::EngineCallPoints::START);

    CallInjections(AppModule::EngineCallPoints::END);
}

void App::CallInjections(AppModule::EngineCallPoints point)
{
    for (auto &inject : module_call_injections[point])
        inject.call();
}

void App::RegisterInjection(const AppModule::EngineCallInject &inject)
{
    if (!module_call_injections.contains(inject.call_point))
        module_call_injections.emplace(inject.call_point, decltype(module_call_injections)::mapped_type());

    auto &entries = module_call_injections[inject.call_point];
    if (entries.empty())
    {
        entries.push_back(inject);
    }
    else
    {
        for (int i = 0; i < entries.size(); i++)
        {
            if (entries[i].priority <= inject.priority)
            {
                entries.insert(entries.begin() + i, inject);
                break;
            }
        }
    }
}

void App::SetupSelfInjections()
{
    RegisterInjection({AppModule::EngineCallPoints::START,INT64_MAX-1,[this](){
        logger.Debug(LandmarkLogoAsciiWName);
        
    }});
}
