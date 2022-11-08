#include <GLFW/glfw3.h>

#include "RendererVulkan.h"
#include "RendererOpenGL.h"

namespace Animation
{
	std::unique_ptr<Renderer> &Renderer::getRenderer()
	{
		// TODO: do not hardcode: find which render is better for given OS
//		return RendererVulkan::getRenderer();
		return RendererOpenGL::getRenderer();
	}

	void Renderer::setWindowFlags()
	{
		// TODO: do not hardcode
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	void Renderer::preRender()
	{

	}

	void Renderer::postRender()
	{

	}

	void Renderer::render() const
	{
		for (const Mesh &mesh: m_Meshes)
		{
			mesh.render();
		}
	}

	void Renderer::addMesh(Mesh mesh)
	{
		m_Meshes.emplace_back(std::move(mesh));
	}
}