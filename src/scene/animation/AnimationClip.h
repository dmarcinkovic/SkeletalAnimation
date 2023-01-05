#ifndef SKELETALANIMATION_ANIMATIONCLIP_H
#define SKELETALANIMATION_ANIMATIONCLIP_H

#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

namespace Animation
{
	class AnimationClip final
	{
	private:
		std::vector<glm::mat4> m_Transforms;
		std::unordered_map<std::string, const class aiNodeAnim *> m_AnimatedNodes;

		float m_AnimationTimeInTicks{0.0f};

		float m_TicksPerSecond{0.0f};
		float m_DurationInTicks{0.0f};

		bool m_AnimationDone{};

	public:
		explicit AnimationClip(const class aiAnimation *animation);

		void play(const aiNode *rootNode, float deltaTimeInSeconds);

		[[nodiscard]] const std::vector<glm::mat4> &getTransforms() const;

		bool isDone() const;

	private:
		glm::mat4 getNodeTransform(const class aiNode *node, const glm::mat4 &parentTransform);

		void updateAnimationTime(float deltaTimeInSeconds);
	};
}

#endif //SKELETALANIMATION_ANIMATIONCLIP_H
