#ifndef SKELETALANIMATION_VERTEXBUFFEROBJECT_H
#define SKELETALANIMATION_VERTEXBUFFEROBJECT_H

#include <vector>
#include <cstdint>

#include "Buffer.h"

namespace Animation
{
	class VertexBufferObject : public Buffer
	{
	protected:
		int m_DataSize{};
		std::uint32_t m_Binding{};

	public:
		~VertexBufferObject() override = default;

		virtual void storeFloatData(std::uint32_t binding, const std::vector<float> &data, int dataSize) = 0;

		virtual void storeIntData(std::uint32_t binding, const std::vector<int> &data, int dataSize) = 0;

		[[nodiscard]] int getDataSize() const;
	};
}

#endif //SKELETALANIMATION_VERTEXBUFFEROBJECT_H
