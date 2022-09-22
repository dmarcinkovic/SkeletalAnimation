#include <spdlog/spdlog.h>

#include "Window.h"
#include "Renderer.h"

namespace Animation
{
	Window &Window::getWindow()
	{
		static Window window{TITLE, WIDTH, HEIGHT};

		return window;
	}

	Window::Window(const char *title, int width, int height)
	{
		if (!glfwInit())
		{
			spdlog::error("Failed to initialize glfw library.");
			std::exit(EXIT_FAILURE);
		}

		setWindowHints();
		createWindow(title, width, height);

		glfwSetWindowSizeCallback(m_Window, windowResizeCallback);

		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	bool Window::isRunning() const
	{
		return !glfwWindowShouldClose(m_Window);
	}

	void Window::clearWindow()
	{
		Renderer::getRenderer()->clearWindow();
	}

	void Window::update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void Window::setWindowHints()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		Renderer::setWindowFlags();
	}

	void Window::createWindow(const char *title, int width, int height)
	{
		m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!m_Window)
		{
			const char *errorMessage;
			glfwGetError(&errorMessage);

			spdlog::error("Failed to create window: '{}'", errorMessage);
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}
	}

	GLFWwindow *Window::getRawWindow()
	{
		assert(m_Window);
		return m_Window;
	}

	void Window::getFramebufferSize(int &width, int &height) const
	{
		assert(m_Window);
		glfwGetFramebufferSize(m_Window, &width, &height);
	}

	void Window::getValidFramebufferSize(int &width, int &height) const
	{
		do
		{
			getFramebufferSize(width, height);
			glfwWaitEvents();
		} while (width == 0 || height == 0);
	}

	void Window::windowResizeCallback(GLFWwindow *, int width, int height)
	{
		Renderer::getRenderer()->setViewport(width, height);
	}
}