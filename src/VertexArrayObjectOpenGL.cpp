#include <cassert>

#include "VertexArrayObjectOpenGL.h"

namespace Animation
{
	VertexArrayObjectOpenGL::VertexArrayObjectOpenGL()
	{
		glGenVertexArrays(1, &m_Id);
		assert(m_Id > 0);
	}

	VertexArrayObjectOpenGL::~VertexArrayObjectOpenGL()
	{
		assert(m_Id > 0);
		glDeleteVertexArrays(1, &m_Id);
	}

	void VertexArrayObjectOpenGL::bind()
	{
		assert(m_Id > 0);
		glBindVertexArray(m_Id);

		for (const auto &vertexBufferObject: m_VertexBufferObjects)
		{
			int attributeIndex = vertexBufferObject.first;
			glEnableVertexAttribArray(attributeIndex);
		}
	}

	void VertexArrayObjectOpenGL::unbind()
	{
		glBindVertexArray(0);

		for (const auto &vertexBufferObject: m_VertexBufferObjects)
		{
			int attributeIndex = vertexBufferObject.first;
			glDisableVertexAttribArray(attributeIndex);
		}
	}

	void VertexArrayObjectOpenGL::storeData(int attributeIndex, std::unique_ptr<VertexBufferObject> vertexBufferObject)
	{
		assert(attributeIndex >= 0);

		m_VertexBufferObjects[attributeIndex] = std::move(vertexBufferObject);
		// TODO: find some better name for this variable
		const std::unique_ptr<VertexBufferObject> &vbo = m_VertexBufferObjects[attributeIndex];

		bind();

		vbo->bind();
		// TODO: do not hardcode GL_FLOAT
		glVertexAttribPointer(attributeIndex, vbo->getDataSize(), GL_FLOAT, GL_FALSE, 0, nullptr);
		vbo->unbind();

		unbind();
	}
}