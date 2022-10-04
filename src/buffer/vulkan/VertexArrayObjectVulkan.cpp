
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
			vertexBuffer.second->bind();
		}
	}

	void VertexArrayObjectVulkan::storeData(int attributeIndex, std::unique_ptr<VertexBufferObject> vertexBufferObject)
	{
		assert(attributeIndex >= 0);
		assert(vertexBufferObject);
		m_VertexBufferObjects[attributeIndex] = std::move(vertexBufferObject);
	}

	void VertexArrayObjectVulkan::setIndexBufferData(const std::vector<std::uint32_t> &indices)
	{
		assert(m_IndexBuffer);
		m_IndexBuffer->storeData(indices);
	}
}