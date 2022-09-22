#include <cassert>
#include <spdlog/spdlog.h>

#include "RenderSurface.h"
#include "Window.h"

namespace Animation
{
	RenderSurface::RenderSurface(VkInstance instance)
			: m_Instance(instance)
	{
		assert(m_Instance != VK_NULL_HANDLE);
		GLFWwindow *window = Window::getWindow().getRawWindow();

		if (glfwCreateWindowSurface(instance, window, nullptr, &m_Surface) != VK_SUCCESS)
		{
			const char *errorDescription;
			glfwGetError(&errorDescription);

			spdlog::error("Failed to create Vulkan render surface: '{}'.", errorDescription);
			std::exit(EXIT_FAILURE);
		}

		assert(m_Surface != VK_NULL_HANDLE);
	}

	RenderSurface::~RenderSurface()
	{
		assert(m_Instance != VK_NULL_HANDLE);
		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
	}

	VkSurfaceKHR RenderSurface::getSurface() const
	{
		assert(m_Surface != VK_NULL_HANDLE);
		return m_Surface;
	}
}