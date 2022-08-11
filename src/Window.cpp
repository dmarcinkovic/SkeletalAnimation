#include <spdlog/spdlog.h>

#include "Window.h"
#include "Renderer.h"

namespace Animation
{
	Window &Window::create(const char *title, int width, int height)
	{
		static Window window{title, width, height};

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
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

	void Window::windowResizeCallback(GLFWwindow *, int width, int height)
	{
		Renderer::getRenderer()->setViewport(width, height);
	}
}