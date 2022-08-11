#include <GL/glew.h>
#include <spdlog/spdlog.h>

#include "RendererOpenGL.h"

namespace Animation
{
	std::unique_ptr<Renderer> &RendererOpenGL::getRenderer()
	{
		static std::unique_ptr<Renderer> renderer(new RendererOpenGL);

		return renderer;
	}

	RendererOpenGL::RendererOpenGL()
	{
		GLenum status = glewInit();

		if (status != GLEW_OK)
		{
			const auto *errorMessage = reinterpret_cast<const char *>(glewGetErrorString(status));

			spdlog::error("Failed to initialize glew library: '{}'.", errorMessage);
			std::exit(EXIT_FAILURE);
		}
	}

	void RendererOpenGL::setViewport(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void RendererOpenGL::clearWindow()
	{
		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}