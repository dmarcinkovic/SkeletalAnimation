#include <cassert>

#include "VertexArrayObjectOpenGL.h"
#include "IndexBufferOpenGL.h"

namespace Animation
{
	VertexArrayObjectOpenGL::VertexArrayObjectOpenGL()
			: VertexArrayObject(std::make_unique<IndexBufferOpenGL>())
	{
		glGenVertexArrays(1, &m_Id);
		assert(m_Id > 0);
	}

	VertexArrayObjectOpenGL::~VertexArrayObjectOpenGL()
	{
		assert(m_Id > 0);
		glDeleteVertexArrays(1, &m_Id);
	}

	void VertexArrayObjectOpenGL::bind() const
	{
		assert(m_Id > 0);
		glBindVertexArray(m_Id);

		for (const auto &vertexBufferObject: m_VertexBufferObjects)
		{
			int attributeIndex = vertexBufferObject.first;
			glEnableVertexAttribArray(attributeIndex);
		}
	}

	void VertexArrayObjectOpenGL::unbind() const
	{
		for (const auto &vertexBufferObject: m_VertexBufferObjects)
		{
			int attributeIndex = vertexBufferObject.first;
			glDisableVertexAttribArray(attributeIndex);
		}

		glBindVertexArray(0);
	}

	void VertexArrayObjectOpenGL::storeData(int attributeIndex, std::unique_ptr<VertexBufferObject> vertexBufferObject)
	{
		assert(attributeIndex >= 0);

		m_VertexBufferObjects[attributeIndex] = std::move(vertexBufferObject);
		const std::unique_ptr<VertexBufferObject> &vbo = m_VertexBufferObjects[attributeIndex];

		bind();
		vbo->bind();
		glVertexAttribPointer(attributeIndex, vbo->getDataSize(), GL_FLOAT, GL_FALSE, 0, nullptr);
		vbo->unbind();
		unbind();
	}

	void VertexArrayObjectOpenGL::setIndexBufferData(const std::vector<std::uint32_t> &indices)
	{
		assert(m_IndexBuffer);

		bind();
		m_IndexBuffer->storeData(indices);
		unbind();
	}
}