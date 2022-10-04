#include <GL/glew.h>
#include <spdlog/spdlog.h>

#include "RendererOpenGL.h"
#include "VertexArrayObjectOpenGL.h"
#include "VertexBufferObjectOpenGL.h"
#include "ShaderOpenGL.h"

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

		spdlog::info("Initialized OpenGL renderer.");
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

	void RendererOpenGL::draw(std::uint64_t vertexCount) const
	{
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertexCount), GL_UNSIGNED_INT, nullptr);
	}

	std::unique_ptr<VertexArrayObject> RendererOpenGL::createVertexArrayObject() const
	{
		return std::make_unique<VertexArrayObjectOpenGL>();
	}

	std::unique_ptr<VertexBufferObject> RendererOpenGL::createVertexBufferObject() const
	{
		return std::make_unique<VertexBufferObjectOpenGL>();
	}

	std::unique_ptr<Shader> RendererOpenGL::getShader() const
	{
		return std::make_unique<ShaderOpenGL>();
	}
}