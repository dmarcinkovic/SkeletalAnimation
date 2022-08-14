#ifndef SKELETALANIMATION_VERTEXARRAYOBJECT_H
#define SKELETALANIMATION_VERTEXARRAYOBJECT_H

#include <unordered_map>
#include <memory>

#include "Buffer.h"
#include "VertexBufferObject.h"
#include "IndexBuffer.h"

namespace Animation
{
	class VertexArrayObject : public Buffer
	{
	protected:
		std::unordered_map<int, std::unique_ptr<VertexBufferObject>> m_VertexBufferObjects;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

	public:
		explicit VertexArrayObject(std::unique_ptr<IndexBuffer> indexBuffer);

		~VertexArrayObject() override = default;

		virtual void storeData(int attributeIndex, std::unique_ptr<VertexBufferObject> vertexBufferObject) = 0;

		virtual void setIndexBufferData(const std::vector<std::uint32_t> &indices) = 0;
	};
}

#endif //SKELETALANIMATION_VERTEXARRAYOBJECT_H
