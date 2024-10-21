#pragma once


#include "IAttachableModule.h"
#include "Debug/Debug.h"
#include "G_API/VulkanInstance.h"
namespace Landmark
{
	struct  ApplicationRunDesc
	{
		std::string AppName;

	};
	class  Application
	{
		
	private:
		static inline Application* Instance = nullptr;

		const bool __LogKeeperInitializer__;
		VulkanInstance vulkanInstance;
		std::vector <IAttachableModule*> EngineModules;
		
		const std::string AppName = "DefaultName";
		const ApplicationRunDesc RunSettings;

		bool ShouldTerminate = false;
		Log logger = Log("Application");
		
		
	public:

		Application(const ApplicationRunDesc&);

		template <typename T, typename... ARGS, typename = std::enable_if_t<std::is_base_of_v<IAttachableModule, T>>>
		T* AttachModule(ARGS&&... args) {

			T* module = new T(std::forward<ARGS>(args)...);
			EngineModules.push_back(module);
			return module;
		}
		virtual void Run();

	private:
		virtual void Start(const ApplicationRunDesc& desc);
		virtual void Update();
		virtual void End();


	};

	
}

