#include "pch.h"
#include "Application.h"
#include "Debug/Debug.h"

//#include "Systems/Renderer/Renderer.h"
#include "Debug/ImguiHandler.h"
#include "Renderer/Renderer.h"
#include "Time/Time.h"
#include "Transform/TransformSystem.h"
#include "Logo.h"
Landmark::Application::Application(const ApplicationRunDesc& desc) :
RunSettings(desc),
AppName(desc.AppName),
__LogKeeperInitializer__([](){LogKeeper::Initialize(LogKeeperInitDesc{}); return true; }()),
vulkanInstance(true)
{
	EngineModules = {};
}

void Landmark::Application::Run()
{
	int a;
	if (Instance)
		throw std::runtime_error("Landmark Application is already active!");
	Instance = this;

	logger.Print('\n'+LandmarkLogoAsciiWName);
	//std::cerr << LandmarkLogoAsciiWName << std::endl;



	Start(RunSettings);

}

void Landmark::Application::Start(const ApplicationRunDesc& desc)
{
	logger.Debug("Internal Init");
	//logger.Debug() << "Internal Init" << Log::end;
	Time::Init();

	Renderer::Init();
	Editor::Init();
	/*
	Graphics::RenderAPI::Initialize(graphicsInitDesc);
	


	
*/

	for (auto& module : EngineModules)
		module->Init();

	Update();
	End();
}

void Landmark::Application::Update()
{

	auto& window = VulkanInstance::GetInstance().GetWindow();
	
	while (!window.ShouldClose())
	{
		Debug::ImguiHandler::NewFrame();

		
		Editor::Draw();
		for (auto& module : EngineModules)
			module->Update();
		
		Renderer::Render();
		glfwPollEvents();
	}
	/*
	auto& graphics = *Graphics::RenderAPI::GetInstance();
	while (!graphics.GetWindow().ShouldClose())
	{
		TransformSystem::Update();
		for (auto& module : EngineModules)
			module->Update();

		

		if (!graphics.GetWindow().isMinimized())
		{
			graphics.PreRender();

			
			for (auto& module : EngineModules)
				module->PreRender();
			Render::Renderer::Render();


			graphics.PostRender();
		}
		graphics.PollEvents();
		
	}
	*/
}

void Landmark::Application::End()
{
	/*
	for (auto& module : EngineModules)
		module->Exit();
		*/
	logger.Debug("Landmark Shutting down");
	//logger << "Landmark Shutting down" << Log::end;
	
}
