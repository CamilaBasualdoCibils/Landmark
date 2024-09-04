#pragma once
#include "Debug/Debug.h"
#include <G_API/Rendering/RenderPass.h>

#include "G_API/Operations/CommandBuffer.h"

#include <G_API/Rendering/Swapchain.h>
#include "G_API/Synchronization/Fence.h"
#include "G_API/Synchronization/Semaphore.h"
#include <optional>
#include "Renderer/Camera.h"
//#include "G_API/BindlessResources.h"
#include "G_API/GLSLCompiler.h"
#include "G_API/Bindless/BindlessDescriptorSet.h"
class Renderer
{

	static inline Log logger = Log("Renderer");
	//c inline std::optional<Graphics::UniformBuffer> meshRendersTransforms;
	//c inline uint64_t LastMeshRenderCount = 1;
	static inline const  int MaxFramesInFlight = 1;
	static inline std::optional<RenderPass> renderpass;
	//static inline std::optional<Swapchain> swapchain;
	static inline std::optional<Swapchain> swapchain;
	static inline std::optional<Semaphore> imageAvailableSemaphore, renderFinishedSemaphore;
	static inline std::optional<Fence> inFlightFence;

	static inline std::optional<CommandBuffer> renderingCommandBuffer;
	static inline std::optional<CommandPool> renderingCommandPool;

	static inline PresentModes CurrentPresentMode;
	static inline ColorFormat currentSurfaceFormat;
	static inline ColorSpace currentColorSpace;

	static inline glm::uvec2 lastWindowResolution;

	//static inline std::optional<BindlessResources> bindlessResources;
	static inline std::optional<BindlessDescriptorSet> bindlessResources;
	static inline GLSLCompiler compiler;
	static inline uint32_t CurrentImageIndex = 0;
	static inline bool RenderSkipped = false;

	static inline std::optional<Framebuffer> RenderFramebuffer;
public:

	static void Init();
	static void Render(); //renders imgui if editor, renders main camera if not
	static void RenderCamera(Camera& camera);
	static void Shutdown();
	static ColorFormat GetCurrentColorFormat() {return currentSurfaceFormat;}
	static ColorSpace GetCurrentColorSpace() {return currentColorSpace;}
	//static vk::SurfaceFormatKHR GetCurrentSurfaceFormat() { return currentSurfaceFormat; }
	static RenderPass& GetRenderPass() { return renderpass.value(); }
private:
	static void RenderEntry();
	static void RenderScene();
	static void RenderExit();
	static PresentModes ChoosePresentMode();
	static std::pair<ColorFormat,ColorSpace> ChooseSurfaceFormat();

	static void RegisterEditorTools();
	static void RecreateSwapchain();
};

