#pragma once
#include "Debug/Debug.h"
struct GLFWwindow;
namespace Landmark
{
	
	class  Window
	{
	public:
		struct CallBacks
		{
			std::function<void(Window*, uvec2)> framebufferResize;
		};
	private:

		static inline Window* MainWindow = nullptr;
		GLFWwindow* glfw_window_ptr = nullptr;
		vk::SurfaceKHR vulkan_surface;
		static inline Log Logger = Log("Window");

		CallBacks callbacks;

	public:
		Window(vk::Instance vk_instance);


		bool ShouldClose();

		vk::SurfaceKHR GetSurface() const;

		glm::uvec2 GetSize() const;

		GLFWwindow* GetGlfwWindowPtr() { return glfw_window_ptr; }

		bool isMinimized();

		void SetCallbacks(const CallBacks&);

		glm::vec2 GetContentScale();

	private:
		void FramebufferCallback( uvec2 newsize);
	};

}
