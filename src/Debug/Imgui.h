#pragma once
#include "Debug/Debug.h"

#include "VK/Operations/CommandBuffer.h"
#include <VK/Rendering/RenderPass.h>
#include <VK/Devices/LogicalDevice.h>
#include <VK/Vulkan_Instance.h>
#include <Core/Window.h>

	class Imgui
	{
		
		ImGui_ImplVulkanH_Window IMGUI_WINDOW;

		Log logger = Log("Imgui");
	public:
		 void Init(Vulkan_Instance& vkinstance,LogicalDevice& device,Queue& queue, RenderPass& rpp,LWindow& window,Format format);

		 void NewFrame();
		 void EndFrame();
		
		 void Render(CommandBuffer& cmdBuffer);
		 void Clear();

	private:
		static void PushEditorTools();
	};

