#include <cassert>

#include "VertexBufferObjectOpenGL.h"

namespace Animation
{
	VertexBufferObjectOpenGL::VertexBufferObjectOpenGL()
	{
		glGenBuffers(1, &m_Id);
		assert(m_Id > 0);
	}

	VertexBufferObjectOpenGL::~VertexBufferObjectOpenGL()
	{
		assert(m_Id > 0);
		glDeleteBuffers(1, &m_Id);
	}

	void VertexBufferObjectOpenGL::bind() const
	{
		assert(m_Id > 0);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	}

	void VertexBufferObjectOpenGL::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBufferObjectOpenGL::storeFloatData(const std::vector<float> &data, int dataSize)
	{
		assert(dataSize > 0);
		m_DataSize = dataSize;

		bind();

		auto bufferSize = static_cast<GLsizeiptr>(data.size() * sizeof(float));
		glBufferData(GL_ARRAY_BUFFER, bufferSize, data.data(), GL_STATIC_DRAW);

		unbind();
	}
}