#include "pch.h"
#include "VulkanInstance.h"


PFN_vkCreateDebugUtilsMessengerEXT  pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT( VkInstance                                 instance,
                                                               const VkDebugUtilsMessengerCreateInfoEXT * pCreateInfo,
                                                               const VkAllocationCallbacks *              pAllocator,
                                                               VkDebugUtilsMessengerEXT *                 pMessenger )
{
  return pfnVkCreateDebugUtilsMessengerEXT( instance, pCreateInfo, pAllocator, pMessenger );
}

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	static Log VulkanLogger = Log("Vulkan");

	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		VulkanLogger.Debug(pCallbackData->pMessage);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		VulkanLogger.Debug(pCallbackData->pMessage);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		VulkanLogger.Warning(pCallbackData->pMessage);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		VulkanLogger.Error(pCallbackData->pMessage);
		break;
	default:;
	}
	return VK_FALSE;
}
VulkanInstance::VulkanInstance(bool _ValidationEnabled) : ValidationEnabled(_ValidationEnabled),
														  instance([&]()
																   {
			if (INSTANCE) log.Critical( "Vulkan Instance initialized twice!");
			INSTANCE = this;
			return CreateInstance(); }()),
														  window(instance),
														  physicalDevices(FetchPhysicalDevices()),
														  mainDevice(PickMainDevice())

{
	vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
		createInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
		createInfo.pfnUserCallback = VulkanDebugCallback;
		createInfo.pUserData = nullptr; // Optional

		pfnVkCreateDebugUtilsMessengerEXT= reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));

		if (!pfnVkCreateDebugUtilsMessengerEXT)
		{
			log.Critical("CreateDebugUtilsMessenger func address not found!");
		}
		auto result = instance.createDebugUtilsMessengerEXT(createInfo);
		//auto result = pfnVkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
		debugMessenger = result.value;

		if (result.result != vk::Result::eSuccess)
			log.Critical("Failed to initialize Debug Messenger");
		else
			log.Debug("Debug Messenger Created");

}

vk::Instance VulkanInstance::CreateInstance()
{
	if (!glfwInit())
	{
		const char *Desc;
		int errorCode = glfwGetError(&Desc);

		log.Critical("GLFW failed to initialize: " + std::string(Desc));
		// log.Critical() << "GLFW failed to initialize:\n" << Desc << Log::end;
	}

	std::vector<const char *> RequestedExtensions;
	std::vector<const char *> RequestedValidationLayers;

	uint32_t glfwExtensionCount = 0;
	const char **glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (int i = 0; i < glfwExtensionCount; i++)
		RequestedExtensions.emplace_back(glfwExtensions[i]);
	RequestedExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

	if (ValidationEnabled)
	{

		RequestedExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		RequestedValidationLayers.emplace_back("VK_LAYER_KHRONOS_validation");
	}

	const int VersionMajor = 1, VersionMinor = 3, VersionPatch = 28;
	vk::ApplicationInfo app_info{};
	app_info.sType = vk::StructureType::eApplicationInfo;
	app_info.pApplicationName = "DefaultAppName";
	app_info.applicationVersion = vk::makeApiVersion(0,1, 0, 0);
	app_info.pEngineName = "Landmark Engine";
	app_info.engineVersion = vk::makeApiVersion(0,1, 0, 0);

	app_info.apiVersion = vk::makeApiVersion(0, VersionMajor, VersionMinor, VersionPatch);

	vk::InstanceCreateInfo instance_create_info{};
	instance_create_info.sType = vk::StructureType::eInstanceCreateInfo;
	instance_create_info.pApplicationInfo = &app_info;
	instance_create_info.flags |= vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
	instance_create_info.enabledExtensionCount = RequestedExtensions.size();
	instance_create_info.ppEnabledExtensionNames = RequestedExtensions.data();
	instance_create_info.enabledLayerCount = RequestedValidationLayers.size();
	instance_create_info.ppEnabledLayerNames = RequestedValidationLayers.data();

	auto result = vk::createInstance(instance_create_info);
	
	if (result.result != vk::Result::eSuccess)
	{
		log.Critical("FAILED TO CREATE VULKAN INSTANCE. ERROR: " + vk::to_string(result.result));
		switch (result.result)
		{
			case vk::Result::eErrorExtensionNotPresent:
			{
				const auto availExt = vk::enumerateInstanceExtensionProperties().value;
				std::set<const char*> aExt;
				for (const auto& a : availExt)
					aExt.insert(a.extensionName.data());
				for (const auto& ext : RequestedExtensions)
					if (!aExt.contains(ext))
						log.Critical("Extension " +std::string( ext) +" not available in this system");
			}
			break;
			case vk::Result::eErrorLayerNotPresent:
			{
				const auto availLay = vk::enumerateInstanceLayerProperties().value;
				std::set<const char*> aLay;
				for (const auto& a : availLay)
					aLay.insert(a.layerName.data());
				for (const auto& lay : RequestedValidationLayers)
					if (!aLay.contains(lay))
						log.Critical("Layer " +std::string( lay) +" not available in this system");
			}
			break;
		
		default:
			break;
		}
	}
	LASSERT(result.result == vk::Result::eSuccess,"VULKAN INSTANCE CREATE FAILURE");
	log.Debug("===== Vulkan Instance  ===== \nExtensions:");
	for (auto &ext : RequestedExtensions)
		log.Debug(std::string("	") + ext);
	log.Debug("Validation Layers:");
	for (auto layer : RequestedValidationLayers)
		log.Debug(std::string("	") + layer);
	log.Debug("============================");
	
	
	return result.value;
}

std::vector<PhysicalDevice> VulkanInstance::FetchPhysicalDevices()
{
	const std::vector<vk::PhysicalDevice> &vkphysicaldevices = instance.enumeratePhysicalDevices().value;

	std::vector<PhysicalDevice> physicalDevices;
	physicalDevices.reserve(vkphysicaldevices.size());
	for (const vk::PhysicalDevice &pd : vkphysicaldevices)
	{
		physicalDevices.emplace_back(pd);
	}
	return physicalDevices;
}

const PhysicalDevice &VulkanInstance::PickMainDevice()
{
	return physicalDevices[0];
}

VulkanInstance &VulkanInstance::GetInstance()
{
	if (!INSTANCE)
		log.Critical("VulkanInstance Does not Exist");
	return *INSTANCE;
}

Landmark::Window &VulkanInstance::GetWindow()
{
	return GetInstance().window;
}
