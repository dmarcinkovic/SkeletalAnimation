
#include <memory>

#include "VertexArrayObjectVulkan.h"
#include "IndexBufferVulkan.h"

namespace Animation
{
	VertexArrayObjectVulkan::VertexArrayObjectVulkan()
			: VertexArrayObject(std::make_unique<IndexBufferVulkan>())
	{

	}

	void VertexArrayObjectVulkan::bind() const
	{
		for (const auto &vertexBuffer: m_VertexBufferObjects)
		{
			vertexBuffer.second->bind();
		}

		m_IndexBuffer->bind();
	}

	void VertexArrayObjectVulkan::unbind() const
	{
		for (const auto &vertexBuffer: m_VertexBufferObjects)
		{
			vertexBuffer.second->unbind();
		}
	}

	void VertexArrayObjectVulkan::storeFloatData(int attributeIndex, std::unique_ptr<VertexBufferObject> vertexBuffer)
	{
		assert(attributeIndex >= 0);
		assert(vertexBuffer);
		m_VertexBufferObjects[attributeIndex] = std::move(vertexBuffer);
	}

	void VertexArrayObjectVulkan::storeIntData(int attributeIndex, std::unique_ptr<VertexBufferObject> vertexBuffer)
	{
		assert(attributeIndex >= 0);
		assert(vertexBuffer);
		m_VertexBufferObjects[attributeIndex] = std::move(vertexBuffer);
	}

	void VertexArrayObjectVulkan::setIndexBufferData(const std::vector<std::uint32_t> &indices)
	{
		assert(m_IndexBuffer);
		m_IndexBuffer->storeData(indices);
	}
}