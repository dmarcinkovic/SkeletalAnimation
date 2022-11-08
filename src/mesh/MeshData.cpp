#include <cassert>

#include "MeshData.h"
#include "Renderer.h"

namespace Animation
{
	MeshData::MeshData(const std::vector<float> &vertices, const std::vector<float> &textureCoordinates,
					   const std::vector<float> &normals, const std::vector<std::uint32_t> &indices)
			: m_VertexCount(indices.size()), m_VertexArrayObject(Renderer::getRenderer()->createVertexArrayObject())
	{
		assert(m_VertexArrayObject);
		m_VertexArrayObject->setIndexBufferData(indices);

		const std::unique_ptr<Renderer> &renderer = Renderer::getRenderer();
		{
			std::unique_ptr<VertexBufferObject> positionData = renderer->createVertexBufferObject();
			positionData->storeFloatData(0, vertices, DATA_SIZE);
			m_VertexArrayObject->storeData(0, std::move(positionData));
		}
		{
			std::unique_ptr<VertexBufferObject> textureData = renderer->createVertexBufferObject();
			textureData->storeFloatData(1, textureCoordinates, 2);
			m_VertexArrayObject->storeData(1, std::move(textureData));
		}
		{
			std::unique_ptr<VertexBufferObject> normalsData = renderer->createVertexBufferObject();
			normalsData->storeFloatData(2, normals, DATA_SIZE);
			m_VertexArrayObject->storeData(2, std::move(normalsData));
		}
	}

	MeshData::MeshData(const std::vector<float> &vertices, const std::vector<std::uint32_t> &indices)
	{
		assert(m_VertexArrayObject);
		m_VertexArrayObject->setIndexBufferData(indices);

		{
			std::unique_ptr<VertexBufferObject> positionData = Renderer::getRenderer()->createVertexBufferObject();
			positionData->storeFloatData(0, vertices, DATA_SIZE);
			m_VertexArrayObject->storeData(0, std::move(positionData));
		}
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