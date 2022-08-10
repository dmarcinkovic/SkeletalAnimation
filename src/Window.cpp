#include <GL/glew.h>
#include <glm/vec4.hpp>
#include <spdlog/spdlog.h>

#include "Window.h"

namespace Animation
{
	Window::Window(const char *title, int width, int height)
	{
		if (!glfwInit())
		{
			spdlog::error("Failed to initialize glfw library.");
			std::exit(EXIT_FAILURE);
		}

		setWindowHints();
		createWindow(title, width, height);

		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);

		if (glewInit() != GLEW_OK)
		{
			spdlog::error("Failed to initialize glew library.");
			std::exit(EXIT_FAILURE);
		}
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
		static const glm::vec4 clearColor{1.0f, 1.0f, 1.0f, 1.0f};

		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void Window::setWindowHints()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	void Window::createWindow(const char *title, int width, int height)
	{
		m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!m_Window)
		{
			spdlog::error("Failed to create window.");
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}
	}
}