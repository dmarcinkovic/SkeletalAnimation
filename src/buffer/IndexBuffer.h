#ifndef SKELETALANIMATION_INDEXBUFFER_H
#define SKELETALANIMATION_INDEXBUFFER_H

#include <vector>
#include <cstdint>

#include "Buffer.h"

namespace Animation
{
	class IndexBuffer : public Buffer
	{
	public:
		~IndexBuffer() override = default;

		virtual void storeData(const std::vector<std::uint32_t> &indices) = 0;
	};

}

#endif //SKELETALANIMATION_INDEXBUFFER_H
