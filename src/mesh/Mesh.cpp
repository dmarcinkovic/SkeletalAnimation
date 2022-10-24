#include "Mesh.h"
#include "Renderer.h"

namespace Animation
{
	Mesh::Mesh(MeshData meshData, Material material)
			: m_MeshData(std::move(meshData)), m_Material(std::move(material))
	{
	}

	void Mesh::render() const
	{
		m_Material.start();

		m_Material.update();
		m_MeshData.bindMesh();

		Renderer::getRenderer()->draw(m_MeshData.getVertexCount());

		m_MeshData.unbindMesh();
		m_Material.end();
	}
}