#include <cassert>

#include "VertexArrayObject.h"

namespace Animation
{
	VertexArrayObject::VertexArrayObject(std::unique_ptr<IndexBuffer> indexBuffer)
			: m_IndexBuffer(std::move(indexBuffer))
	{
		assert(m_IndexBuffer);
	}
}