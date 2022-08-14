#include <cassert>

#include "IndexBufferOpenGL.h"

namespace Animation
{
	IndexBufferOpenGL::IndexBufferOpenGL()
	{
		glGenBuffers(1, &m_Id);
		assert(m_Id > 0);
	}

	IndexBufferOpenGL::~IndexBufferOpenGL()
	{
		assert(m_Id > 0);
		glDeleteBuffers(1, &m_Id);
	}

	void IndexBufferOpenGL::bind()
	{
		assert(m_Id > 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	}

	void IndexBufferOpenGL::unbind()
	{
		assert(m_Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBufferOpenGL::storeData(const std::vector<std::uint32_t> &indices)
	{
		bind();

		auto size = static_cast<GLsizeiptr>(indices.size() * sizeof(std::uint32_t));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices.data(), GL_STATIC_DRAW);
	}
}