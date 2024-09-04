#include "pch.h"
#include "ImguiHandler.h"

#include "EditorLambda.h"
#include "G_API/Window.h"
#include "G_API/VulkanInstance.h"
#include "G_API/Rendering/RenderPass.h"
#include <Renderer/Renderer.h>
#include "Editor.h"
void Landmark::Debug::ImguiHandler::Init(Queue &queue, RenderPass &rp)
{
	
	auto &VKInstance = VulkanInstance::GetInstance();
	auto &device = VKInstance.GetMainDevice();
	VkDescriptorPoolSize pool_sizes[] =
		{
			{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
			{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
			{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
			{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
			{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
			{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
			{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
			{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
			{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000;
	pool_info.poolSizeCount = std::size(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;

	VkDescriptorPool imguiPool = device->createDescriptorPool(pool_info).value;
	// VK_CHECK(vkCreateDescriptorPool(_device, &pool_info, nullptr, &imguiPool));
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	//(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	  // Enable Multi-Viewport / Platform Windows
	io.FontGlobalScale = VulkanInstance::GetInstance().GetWindow().GetContentScale().x;
	io.FontAllowUserScaling = true;
	ImGuiStyle &style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	ImPlot::CreateContext();
	const auto capabilties = device.GetSurfaceCapabilities(VulkanInstance::GetInstance().GetWindow().GetSurface());
	
	ImGui_ImplGlfw_InitForVulkan(VKInstance.GetWindow().GetGlfwWindowPtr(), true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = VKInstance.GetVkInstance();
	init_info.PhysicalDevice = device.GetVkPhyDevice();
	init_info.Device = device.GetVkDevice();
	init_info.Queue = device.GetGraphicsQueue().GetQueue();
	init_info.DescriptorPool = imguiPool;
	init_info.MinImageCount = capabilties.minImageCount;
	init_info.ImageCount = std::max(capabilties.maxImageCount, std::max(capabilties.minImageCount + 1, uint32_t(3)));
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.ColorAttachmentFormat = static_cast<VkFormat>(Renderer::GetCurrentColorFormat());

	CommandPool cmdPool(queue);
	CommandBuffer cmdBuffer(cmdPool);
	cmdBuffer.Begin(CommandBufferUsageFlags::ONE_TIME_SUBMIT);
	ImGui_ImplVulkan_Init(&init_info, rp.GetVkRenderPass());
	{
		// Graphics::SingleUseCommandBuffer sucm;
		ImGui_ImplVulkan_CreateFontsTexture(static_cast<vk::CommandBuffer>(cmdBuffer));
	}
	cmdBuffer.End();
	cmdBuffer.SubmitAndWait();
	ImGui_ImplVulkan_DestroyFontUploadObjects();

	cmdBuffer.Destroy();
	cmdPool.Destroy();

	PushEditorTools();
	logger.Debug("Init");
	// logger << "Init" << Log::end;
}

void Landmark::Debug::ImguiHandler::NewFrame()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
}

void Landmark::Debug::ImguiHandler::EndFrame()
{
	ImGui::EndFrame();
	if (ImGui::GetDrawData())
		ImGui::GetDrawData()->Clear();

	auto &io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
	}
}

void Landmark::Debug::ImguiHandler::Render(CommandBuffer &cmdBuffer)
{
	ImGui::Render();

	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),static_cast<vk::CommandBuffer>( cmdBuffer));
	auto &io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::RenderPlatformWindowsDefault();
	}
}

void Landmark::Debug::ImguiHandler::Clear()
{
	
}

void Landmark::Debug::ImguiHandler::PushEditorTools()
{

	auto &imguiGroup = Editor::GetMainToolGroup().PushObject<EditorObjectGroup>(5, "ImGui");
	static bool ShowImguiDemo = false;
	static bool ShowImplotDemo = false;

	imguiGroup.PushObject<EditorLambda>(0, "Imgui Demo", [&]()
										{ ImGui::MenuItem("Imgui Demo", 0, &ShowImguiDemo); }, [&]()
										{ if (ShowImguiDemo) ImGui::ShowDemoWindow(); });
	imguiGroup.PushObject<EditorLambda>(1, "Implot Demo", [&]()
										{ ImGui::MenuItem("Implot Demo", 0, &ShowImplotDemo); }, [&]()
										{ if (ShowImplotDemo) ImPlot::ShowDemoWindow(); });
}
