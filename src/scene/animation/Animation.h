#ifndef SKELETALANIMATION_ANIMATION_H
#define SKELETALANIMATION_ANIMATION_H

#include <vector>

#include "AnimationClip.h"

namespace Animation
{
	class Animation final
	{
	private:
		std::size_t m_AnimationClipIndex = 0;

		std::vector<AnimationClip> m_Clips;

	public:
		explicit Animation(const class aiScene *scene);

		void play(const class aiNode *rootNode, float deltaTimeInSeconds);

		[[nodiscard]] const std::vector<glm::mat4> &getTransforms() const;

		[[nodiscard]] bool hasAnimation() const;
	};
}

#endif //SKELETALANIMATION_ANIMATION_H
