#ifndef SKELETALANIMATION_VERTEXBUFFEROBJECT_H
#define SKELETALANIMATION_VERTEXBUFFEROBJECT_H

#include <vector>

#include "Buffer.h"

namespace Animation
{
	class VertexBufferObject : public Buffer
	{
	protected:
		int m_DataSize{};

	public:
		~VertexBufferObject() override = default;

		virtual void storeFloatData(const std::vector<float> &data, int dataSize) = 0;

		[[nodiscard]] int getDataSize() const;
	};
}

#endif //SKELETALANIMATION_VERTEXBUFFEROBJECT_H
