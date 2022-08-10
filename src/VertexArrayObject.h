#ifndef SKELETALANIMATION_VERTEXARRAYOBJECT_H
#define SKELETALANIMATION_VERTEXARRAYOBJECT_H

#include <unordered_map>
#include <memory>

#include "Buffer.h"
#include "VertexBufferObject.h"

namespace Animation
{
	class VertexArrayObject : public Buffer
	{
	protected:
		std::unordered_map<int, std::unique_ptr<VertexBufferObject>> m_VertexBufferObjects;

	public:
		~VertexArrayObject() override = default;

		virtual void storeData(int attributeIndex, std::unique_ptr<VertexBufferObject> vertexBufferObject) = 0;
	};
}

#endif //SKELETALANIMATION_VERTEXARRAYOBJECT_H
