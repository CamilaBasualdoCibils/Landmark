#include "pch.h"
#include "Renderer.h"

#include "Debug/Editor.h"
#include "EditorTools/EditorViewport.h"
#include "G_API/VulkanInstance.h"
#include "G_API/Images/Image.h"
#include "Debug/ImguiHandler.h"
#include "G_API/Buffers/Buffer.h"

#include "G_API/Rendering/Pipeline.h"
#include <IO/IO.h>
#include <IO/File.h>
#include <IO/Folder.h>
void Renderer::Init()
{

	imageAvailableSemaphore.emplace();
	renderFinishedSemaphore.emplace();
	inFlightFence.emplace(true);

	renderingCommandPool.emplace(VulkanInstance::GetInstance().GetMainDevice().GetGraphicsQueue());
	renderingCommandBuffer.emplace(renderingCommandPool.value());
	const auto sf = ChooseSurfaceFormat();
	currentSurfaceFormat = sf.first;
	currentColorSpace = sf.second;
	CurrentPresentMode = ChoosePresentMode();
	renderpass.emplace(currentSurfaceFormat);
	{

		//Swapchain::SwapchainInfo swapchain_info;
		Swapchain::SwapchainProperties swapProperties;

		{
			const auto capabilties = VulkanInstance::GetInstance().GetMainDevice().GetSurfaceCapabilities(VulkanInstance::GetInstance().GetWindow().GetSurface());
			swapProperties.clipped = true;
			swapProperties.alphaMode = CompositeAlphaModes::OPAQUE;
			swapProperties.colorFormat = currentSurfaceFormat;
			swapProperties.transform = EnumCast( capabilties.currentTransform);
			swapProperties.presentMode = CurrentPresentMode;

			if (capabilties.currentExtent.width != std::numeric_limits<uint32_t>::max())
				swapProperties.extent = {capabilties.currentExtent.width, capabilties.currentExtent.height};
			else
			{

				const auto framebufferSize = VulkanInstance::GetInstance().GetWindow().GetSize();

				VkExtent2D actualExtent = {
					static_cast<uint32_t>(framebufferSize.x),
					static_cast<uint32_t>(framebufferSize.y)};

				actualExtent.width = std::clamp(actualExtent.width, capabilties.minImageExtent.width, capabilties.maxImageExtent.width);
				actualExtent.height = std::clamp(actualExtent.height, capabilties.minImageExtent.height, capabilties.maxImageExtent.height);
				swapProperties.extent = {actualExtent.width, actualExtent.height};
			}

			swapProperties.imageCount = std::max(capabilties.maxImageCount, std::max(capabilties.minImageCount + 1, uint32_t(3)));
		}
		
		swapchain.emplace(swapProperties,renderpass.value(), VulkanInstance::GetInstance().GetWindow().GetSurface());
	}

	lastWindowResolution = VulkanInstance::GetInstance().GetWindow().GetSize();

	Landmark::Debug::ImguiHandler::Init(VulkanInstance::GetInstance().GetMainDevice().GetGraphicsQueue(), renderpass.value());

	RegisterEditorTools();

	Framebuffer::FramebufferProperties framebufferProp;
	framebufferProp.dimensions = {300,300,1};
	Framebuffer::FramebufferAttachmentProperties att1;
	att1.format = currentSurfaceFormat;
	att1.tilling = MemoryTillingType::OPTIMAL;
	framebufferProp.attachments.push_back(att1);
	RenderFramebuffer.emplace(framebufferProp,renderpass.value());
}

void Renderer::Render()
{
	RenderEntry();
	RenderScene();
	RenderExit();
}

void Renderer::RenderCamera(Camera &camera)
{
	
}

void Renderer::Shutdown()
{
	renderFinishedSemaphore->Destroy();
	renderFinishedSemaphore.reset();
	imageAvailableSemaphore->Destroy();
	imageAvailableSemaphore.reset();
	inFlightFence->Destroy();
	inFlightFence.reset();
}

void Renderer::RenderEntry()
{

	auto &window = VulkanInstance::GetInstance().GetWindow();

	const bool RenderCapable = !(window.isMinimized() || window.GetSize() == glm::uvec2{0, 0});

	if (!RenderSkipped) // if last frame was not skipped then wait for it to finish
		inFlightFence->WaitAndReset();

	if (!RenderCapable)
	{
		RenderSkipped = true;
		return;
	}
	// NOT OPTIMAL
	// https://vulkan-tutorial.com/Drawing_a_triangle/Swap_chain_recreation

		renderingCommandBuffer->Reset();

	renderingCommandBuffer->Begin();


	renderingCommandBuffer->BeginRenderPass(renderpass.value(),RenderFramebuffer.value(),{{0,0},{300,300}});

	

}

void Renderer::RenderScene()
{
	static PipelineLayout pl;
	Pipeline::PipelineProperties pp;
	pp.shaderStages[ShaderStage::VERTEX] = IO::GetResources().GetFolder("shaders").GetFile("test.vert.spv").GetContent_Binary();
	pp.shaderStages[ShaderStage::FRAGMENT] = IO::GetResources().GetFolder("shaders").GetFile("test.frag.spv").GetContent_Binary();
	static Pipeline pipeline(pp,pl,renderpass.value());
	renderingCommandBuffer->BindPipeline(pipeline,PipelineBindPoint::GRAPHICS);
	renderingCommandBuffer->Draw(5,1);
}

void Renderer::RenderExit()
{
	renderingCommandBuffer->EndRenderPass();


	CurrentImageIndex = swapchain->AcquireNextImage(imageAvailableSemaphore);
	bool SwapchainOutOfDate = swapchain->GetIsOutOfDate();
	RenderSkipped = SwapchainOutOfDate || RenderSkipped;
	if (SwapchainOutOfDate)
	{
		Landmark::Debug::ImguiHandler::EndFrame();
		Landmark::Debug::ImguiHandler::Clear();
		RecreateSwapchain();
		return; // you want this here so that incase acquireNextImage fails and returns something bogus
	}

	const glm::uvec2 windowSize = VulkanInstance::GetInstance().GetWindow().GetSize();

	uvec2 extent = swapchain->GetExtent();
	renderingCommandBuffer->BeginRenderPass(renderpass.value(), swapchain->GetFramebuffer(CurrentImageIndex), {{0, 0}, {extent.x, extent.y}});
	renderingCommandBuffer.value()->setViewport(0, {vk::Viewport{0, 0, float(windowSize.x), float(windowSize.y), 0, 1}});
	renderingCommandBuffer.value()->setScissor(0, {vk::Rect2D{{0, 0}, {windowSize.x, windowSize.y}}});


#if defined(_EDITOR) || defined(_MINIMAL)
	if (!RenderSkipped)
	{
		Landmark::Debug::ImguiHandler::EndFrame();
		Landmark::Debug::ImguiHandler::Render(renderingCommandBuffer.value());
	}
	else
	{
		Landmark::Debug::ImguiHandler::EndFrame();
		Landmark::Debug::ImguiHandler::Clear();
		return;
	}
#else
// Implement this
// RenderCamera(Camera::Main);
#endif

	renderingCommandBuffer->EndRenderPass();
	renderingCommandBuffer->End();

	renderingCommandBuffer->Submit({imageAvailableSemaphore.value()}, {renderFinishedSemaphore.value()}, inFlightFence);

	static auto &presentQueue = VulkanInstance::GetInstance().GetMainDevice().GetPresentQueue();

	presentQueue.Present(CurrentImageIndex, &SwapchainOutOfDate, swapchain.value(), {renderFinishedSemaphore.value()});
	if (SwapchainOutOfDate)
		RecreateSwapchain();
}

PresentModes Renderer::ChoosePresentMode()
{
	const auto presentModes = VulkanInstance::GetInstance().GetMainDevice().GetPresentModes(VulkanInstance::GetInstance().GetWindow().GetSurface());
	const std::vector<PresentModes> IdealPresents = {
		PresentModes::FIFO_RELAXED,
		PresentModes::MAILBOX,
		PresentModes::FIFO

	};

	
	for (auto &ideal : IdealPresents)
	{
		for (const auto& availMode : presentModes) {
			if (availMode == EnumCast(ideal) )
			return ideal;
		}
	}
	return {};
}

std::pair<ColorFormat,ColorSpace> Renderer::ChooseSurfaceFormat()
{
	const auto surfaceFormats = VulkanInstance::GetInstance().GetMainDevice().GetSurfaceFormats(VulkanInstance::GetInstance().GetWindow().GetSurface());
	const std::vector<std::pair<ColorFormat,ColorSpace>> IdealFormats = {
		//{vk::Format::eR32G32B32A32Sfloat,},
		{ColorFormat::BGRA8unorm, ColorSpace::SRGB_NON_LINEAR},
		{ColorFormat::BGRA8SRGB, ColorSpace::SRGB_NON_LINEAR},
		{ColorFormat::RGBA8uint, ColorSpace::SRGB_NON_LINEAR},
		{ColorFormat::RGBA8SRGB, ColorSpace::SRGB_NON_LINEAR}

	};

	
	for (auto &ideal : IdealFormats)
	{
		for (const auto& availFormat : surfaceFormats) {
			if (availFormat.format == EnumCast(ideal.first) &&availFormat.colorSpace == EnumCast(ideal.second))
			return ideal;
		}
		
	}
	
	return {};
}

void Renderer::RegisterEditorTools()
{
	auto &windowGroup = Editor::GetMainToolGroup().GetOrPushGroup("Windows");

	windowGroup.PushObject<EditorViewport>(0);
}

void Renderer::RecreateSwapchain()
{
	const auto capabilties = VulkanInstance::GetInstance().GetMainDevice().GetSurfaceCapabilities(VulkanInstance::GetInstance().GetWindow().GetSurface());
	auto result = VulkanInstance::GetInstance().GetMainDevice()->waitIdle();
	// renderingCommandBuffer->Reset();
	logger.Debug("Recreating Main Swapchain");
	Swapchain::SwapchainProperties info = swapchain->GetProperties();
	info.extent = {capabilties.currentExtent.width, capabilties.currentExtent.height};
	// info.extent = VulkanInstance::GetInstance().GetWindow().GetSize();
	swapchain->Recreate(info);
}
