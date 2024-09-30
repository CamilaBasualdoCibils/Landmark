#include "pch.h"
#include "Window.h"
//#include "RenderAPI.h"
auto GetPointer = [](GLFWwindow* w)
{
	return static_cast<Landmark::Window*>(glfwGetWindowUserPointer(w));
};
Landmark::Window::Window(vk::Instance vk_instance)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, true);
	glm::ivec2 MonitorMin, MonitorSize;
	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &MonitorMin.x, &MonitorMin.y, &MonitorSize.x, &MonitorSize.y);
	glfw_window_ptr = glfwCreateWindow(1920, 1080, "Default Window name", nullptr, nullptr);
	//glfwSetWindowPos(glfw_window_ptr, MonitorMin.x, MonitorMin.y);
	if (glfw_window_ptr)
		Logger.Debug( "Window Created");
	else Logger.Critical("Failed to create Window");

	glfwSetWindowUserPointer(glfw_window_ptr, this);
	std::cout << std::hex;
	const unsigned char* byteMem = reinterpret_cast<const unsigned char*>(&vk_instance);
	for (int i = 0; i < sizeof(vk::Instance);i++) {
		std::cout << std::setw(2) << std::setfill('0')
                  << static_cast<int>(byteMem[i]) << " ";
	}
	std::cout << std::dec;
	VkSurfaceKHR surf;
	glfwCreateWindowSurface(vk_instance, glfw_window_ptr, nullptr, &surf);
	vulkan_surface = vk::SurfaceKHR(surf);

	if (!MainWindow)
	{
		MainWindow = this;
		std::stringstream ss;
		ss << "at " << this << " assigned as Main Window";
		Logger.Debug(ss);
	};


	glfwSetFramebufferSizeCallback(glfw_window_ptr, [](GLFWwindow* w, int x, int y)
		{
			GetPointer(w)->FramebufferCallback({ x,y });
		});
}



bool Landmark::Window::ShouldClose()
{
	return glfwWindowShouldClose(glfw_window_ptr);
}

vk::SurfaceKHR Landmark::Window::GetSurface() const
{
	return vulkan_surface;
}

glm::uvec2 Landmark::Window::GetSize() const
{
	glm::ivec2 size;
	glfwGetFramebufferSize(glfw_window_ptr, &size.x, &size.y);
	return size;
}

bool Landmark::Window::isMinimized()
{
	return glfwGetWindowAttrib(glfw_window_ptr, GLFW_ICONIFIED);
}

void Landmark::Window::SetCallbacks(const CallBacks& o_backs)
{
	callbacks = o_backs;
}

glm::vec2 Landmark::Window::GetContentScale()
{
	glm::vec2 ContentScale;
	glfwGetWindowContentScale(glfw_window_ptr, &ContentScale.x, &ContentScale.y);
	return ContentScale;
}

void Landmark::Window::FramebufferCallback(uvec2 newsize)
{
	if (callbacks.framebufferResize) callbacks.framebufferResize(this, newsize);
}
