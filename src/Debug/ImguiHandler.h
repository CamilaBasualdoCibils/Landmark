#pragma once
#include "Debug/Debug.h"

#include "VK/Operations/CommandBuffer.h"

namespace Landmark::Debug
{
	class ImguiHandler
	{
		
		static inline ImGui_ImplVulkanH_Window IMGUI_WINDOW;

		static inline Log logger = Log("Imgui");
	public:
		static void Init(Queue& queue, RenderPass& rpp);

		static void NewFrame();
		static void EndFrame();
		
		static void Render(CommandBuffer& cmdBuffer);
		static void Clear();

	private:
		static void PushEditorTools();
	};

}
