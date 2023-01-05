#include <GLFW/glfw3.h>

#include "RendererVulkan.h"
#include "RendererOpenGL.h"
#include "Renderer.h"

namespace Animation
{
	namespace
	{
		Renderer::RendererType rendererType = Renderer::RendererType::OPENGL;
	}

	std::unique_ptr<Renderer> &Renderer::getRenderer()
	{
		if (rendererType == RendererType::OPENGL)
		{
			return RendererOpenGL::getRenderer();
		}

		return RendererVulkan::getRenderer();
	}

	void Renderer::setWindowFlags()
	{
		if (rendererType == RendererType::OPENGL)
		{
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		} else
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		}
	}

	void Renderer::preRender()
	{

	}

	void Renderer::postRender()
	{

	}

	void Renderer::setRendererType(Renderer::RendererType type)
	{
		rendererType = type;
	}
}