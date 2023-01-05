#ifndef SKELETALANIMATION_INTERPOLATION_H
#define SKELETALANIMATION_INTERPOLATION_H

#include <glm/glm.hpp>

namespace Animation
{
	class Interpolation final
	{
	public:
		Interpolation() = delete;

		static glm::mat4 getTransform(float animationTimeInTicks, const class aiNodeAnim *nodeAnim);
	};
}

#endif //SKELETALANIMATION_INTERPOLATION_H
