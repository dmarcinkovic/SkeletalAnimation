#include <cassert>

#include "MeshData.h"
#include "Renderer.h"

namespace Animation
{
	MeshData::MeshData(const std::vector<float> &vertices, const std::vector<std::uint32_t> &indices)
			: m_VertexCount(indices.size()), m_VertexArrayObject(Renderer::getRenderer()->createVertexArrayObject())
	{
		assert(m_VertexArrayObject);
		m_VertexArrayObject->setIndexBufferData(indices);

		std::unique_ptr<VertexBufferObject> positionData = Renderer::getRenderer()->createVertexBufferObject();
		positionData->storeFloatData(vertices, DATA_SIZE);

		m_VertexArrayObject->storeData(0, std::move(positionData));
	}

	std::uint64_t MeshData::getVertexCount() const
	{
		return m_VertexCount;
	}

	void MeshData::bindMesh() const
	{
		assert(m_VertexArrayObject);
		m_VertexArrayObject->bind();
	}

	void MeshData::unbindMesh() const
	{
		assert(m_VertexArrayObject);
		m_VertexArrayObject->unbind();
	}
}