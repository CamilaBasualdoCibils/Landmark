#include "App.h"
#include "Logo.h"
App::App()
{
}

void App::Run(const AppProperties &_properties)
{
    instance = this;
    if (_properties.default_modules)
    {
        SetupSelfInjections();
        log_keeper = AttachModule<LogKeeper2>();
        // log_keeper->
    }

    CallInjections(AppModule::EngineCallPoints::START);
    while (!should_terminate)
    {
        glfwPollEvents();
        if (main_window.has_value())
            should_terminate |= main_window->isClosing();

        CallInjections(AppModule::EngineCallPoints::UPDATE);
        CallInjections(AppModule::EngineCallPoints::RENDER);
    }

    CallInjections(AppModule::EngineCallPoints::END);
}

void App::CallInjections(AppModule::EngineCallPoints point)
{
    for (auto &inject : module_call_injections[point])
        inject.call();
}

void App::RegisterInjection(const AppModule::EngineCallInject &inject)
{
    LASSERT(inject.call_point != AppModule::EngineCallPoints::INVALID, "Invalid Inject Point");
    if (!module_call_injections.contains(inject.call_point))
        module_call_injections.emplace(inject.call_point, decltype(module_call_injections)::mapped_type());

    auto &entries = module_call_injections[inject.call_point];
    entries.push_back(inject);
    std::sort(entries.begin(), entries.end(),
              [](const AppModule::EngineCallInject &a, const AppModule::EngineCallInject &b)
              { return a.priority > b.priority; });
}

void App::SetupSelfInjections()
{

    AppModule::EngineCallInject logo_display("logo_display");
    logo_display.call = [this]()
    { logger.Debug(LandmarkLogoAsciiWName); };
    logo_display.call_point = AppModule::EngineCallPoints::START;
    logo_display.priority = INT64_MAX - 1;
    RegisterInjection(logo_display);

    AppModule::EngineCallInject main_window_init_inject("Main Window Init");
    main_window_init_inject.call = [this]()
    { main_window.emplace(Window::Window_Properties()); };
    main_window_init_inject.call_point = AppModule::EngineCallPoints::START;
    main_window_init_inject.priority = 0;
    RegisterInjection(main_window_init_inject);

    AppModule::EngineCallInject main_vulkan_init_inject("Vulkan Instance Init");
    main_vulkan_init_inject.call = std::bind(&App::InitializeVulkan, this);
    main_vulkan_init_inject.call_point = AppModule::EngineCallPoints::START;
    main_vulkan_init_inject.priority = 0;
    RegisterInjection(main_vulkan_init_inject);

    AppModule::EngineCallInject main_window_exit_inject("Main Window destroy");
    main_window_exit_inject.call = [this]()
    { main_window.reset(); };
    main_window_exit_inject.call_point = AppModule::EngineCallPoints::END;
    main_window_exit_inject.priority = 0;
    RegisterInjection(main_window_exit_inject);

    AppModule::EngineCallInject main_vulkan_exit_inject("Vulkan Instance destroy");
    main_vulkan_exit_inject.call = std::bind(&App::CleanUpVulkan,this);
    main_vulkan_exit_inject.call_point = AppModule::EngineCallPoints::END;
    main_vulkan_exit_inject.priority = -1;
    RegisterInjection(main_vulkan_exit_inject);

    AppModule::EngineCallInject main_vulkan_device_init_inject("Vulkan Main Device Init");
    main_vulkan_device_init_inject.call = std::bind(&App::SetupMainVkDevice,this);
    main_vulkan_device_init_inject.call_point = AppModule::EngineCallPoints::START;
    main_vulkan_device_init_inject.priority = -1;
    RegisterInjection(main_vulkan_device_init_inject);
}

void App::InitializeVulkan()
{
    Vulkan_Instance::VulkanInstanceProperties vulkan_properties;
    main_vk_instance.emplace(vulkan_properties);
}

void App::CleanUpVulkan()
{
    main_vk_device.reset();
    main_vk_instance.reset();
}

void App::SetupMainVkDevice()
{
    LASSERT(main_window.has_value(),"Setup of main device requires a window present");
    PhysicalDevice chosen_device = main_vk_instance->EnumerateDevices(&main_window.value())[0];
    main_vk_device.emplace(chosen_device);
}
