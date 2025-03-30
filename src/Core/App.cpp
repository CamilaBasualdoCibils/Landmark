#include "App.h"
#include "Logo.h"
#include <Editor/RenderDoc/RenderDoc.h>
#include <ECS/SceneManager.hpp>
#include <Assets/AssetManager.hpp>
#include <Editor/Editor.h>
App::App()
{
}

void App::Run(const AppProperties &_properties)
{
    instance = this;
    if (_properties.default_modules)
    {
        SetupSelfInjections();
        AttachModule<LogKeeper2>();
        AttachModule<Renderer>();
        AttachModule<SceneManager>();
        AttachModule<AssetManager>();
        AttachModule<Editor>();
    }
    main_window = std::make_shared<LWindow>(LWindow::LWindow_Properties()); 
    InitializeVulkan();
    SetupMainVkDevice();
    CallInjections(AppModule::EngineCallPoints::START);

    #ifdef RENDERDOC_CAPTURE
    RenderDoc::Init();
    #endif

    while (!should_terminate)
    {
        glfwPollEvents();

        if (main_window.get())
            should_terminate |= main_window->isClosing();

        CallInjections(AppModule::EngineCallPoints::UPDATE);
        #ifdef RENDERDOC_CAPTURE
        RenderDoc::SetActiveWindow(*main_vk_instance,*main_window.get());
        RenderDoc::StartCapture();
        #endif
        CallInjections(AppModule::EngineCallPoints::RENDER);
        #ifdef RENDERDOC_CAPTURE
        RenderDoc::EndCapture();
        #endif
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



    AppModule::EngineCallInject main_window_exit_inject("Main LWindow destroy");
    main_window_exit_inject.call = [this]()
    { main_window.reset(); };
    main_window_exit_inject.call_point = AppModule::EngineCallPoints::END;
    main_window_exit_inject.priority = 0;
    RegisterInjection(main_window_exit_inject);

    AppModule::EngineCallInject main_vulkan_exit_inject("Vulkan Instance destroy");
    main_vulkan_exit_inject.call = std::bind(&App::CleanUpVulkan, this);
    main_vulkan_exit_inject.call_point = AppModule::EngineCallPoints::END;
    main_vulkan_exit_inject.priority = -1;
    RegisterInjection(main_vulkan_exit_inject);

    AppModule::EngineCallInject renderer_init("Renderer init");
    renderer_init.call = [this](){GetModule<Renderer>()->Init();};
    renderer_init.call_point = AppModule::EngineCallPoints::START;
    renderer_init.priority = -2;
    RegisterInjection(renderer_init);
}

void App::InitializeVulkan()
{
    Vulkan_Instance::VulkanInstanceProperties vulkan_properties;
    Version4 vk_version;
    vk_version.major = 1;
    vk_version.minor = 4;
    vk_version.patch = 303;
    vulkan_properties.vulkan_version = vk_version;
    main_vk_instance = std::make_shared<Vulkan_Instance>(vulkan_properties);
}

void App::CleanUpVulkan()
{
    main_vk_device.reset();
    main_vk_instance.reset();
}

void App::SetupMainVkDevice()
{
    LASSERT(main_window.get(), "Setup of main device requires a window present");
    const auto& available_devices = main_vk_instance->EnumerateDevices(main_window.get());
    PhysicalDevice chosen_device = available_devices[0];
    main_vk_device = std::make_shared<MainDevice>(chosen_device);
}
