#include "Mesh.h"
#include "Renderer.h"

namespace Animation
{
	Mesh::Mesh(MeshData meshData, std::shared_ptr<Material> material)
			: m_MeshData(std::move(meshData)), m_Material(std::move(material))
	{
	}

	void Mesh::render(Uniform::UniformData uniformData) const
	{
		m_Material->start();

		uniformData.modelMatrix *= m_WorldTransform;
		m_Material->update(uniformData);
		m_MeshData.bindMesh();

		Renderer::getRenderer()->draw(m_MeshData.getVertexCount());

		m_MeshData.unbindMesh();
		m_Material->end();
	}

	void Mesh::setWorldTransform(const glm::mat4 &worldTransform)
	{
		m_WorldTransform = worldTransform;
	}
}