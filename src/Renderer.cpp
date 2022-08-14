#include "Renderer.h"
#include "RendererOpenGL.h"

namespace Animation
{
	std::unique_ptr<Renderer> &Renderer::getRenderer()
	{
		// TODO: do not hardcode: find which render is better for given OS
		return RendererOpenGL::getRenderer();
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