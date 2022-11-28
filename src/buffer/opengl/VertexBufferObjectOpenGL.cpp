
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

	void VertexBufferObjectOpenGL::storeFloatData(std::uint32_t binding, const std::vector<float> &data, int dataSize)
	{
		assert(dataSize > 0);
		m_DataSize = dataSize;
		m_Binding = binding;

		storeData(data.data(), static_cast<GLsizeiptr>(data.size() * sizeof(float)));
	}

	void VertexBufferObjectOpenGL::storeIntData(std::uint32_t binding, const std::vector<int> &data, int dataSize)
	{
		assert(dataSize > 0);
		m_DataSize = dataSize;
		m_Binding = binding;

		storeData(data.data(), static_cast<GLsizeiptr>(data.size() * sizeof(int)));
	}

	void VertexBufferObjectOpenGL::storeData(const void *data, GLsizeiptr bufferSize) const
	{
		bind();
		glBufferData(GL_ARRAY_BUFFER, bufferSize, data, GL_STATIC_DRAW);
		unbind();
	}
}