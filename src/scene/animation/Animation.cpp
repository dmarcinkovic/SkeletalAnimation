#include <assimp/scene.h>
#include <spdlog/spdlog.h>

#include "Animation.h"

namespace Animation
{
	Animation::Animation(const aiScene *scene)
	{
		if (scene->HasAnimations())
		{
			spdlog::info("Found {} animations.", scene->mNumAnimations);

			for (int i = 0; i < scene->mNumAnimations; ++i)
			{
				const aiAnimation *animation = scene->mAnimations[i];
				assert(animation);

				m_Clips.emplace_back(animation);
			}
		} else
		{
			spdlog::info("No animation found.");
		}
	}

	void Animation::play(const aiNode *rootNode, float deltaTimeInSeconds)
	{
		if (!m_Clips.empty())
		{
			AnimationClip &clip = m_Clips[m_AnimationClipIndex];
			clip.play(rootNode, deltaTimeInSeconds);

			if (clip.isDone())
			{
				++m_AnimationClipIndex;
			}

			m_AnimationClipIndex %= m_Clips.size();
		}
	}

	const std::vector<glm::mat4> &Animation::getTransforms() const
	{
		assert(hasAnimation());

		return m_Clips[m_AnimationClipIndex].getTransforms();
	}

	bool Animation::hasAnimation() const
	{
		return !m_Clips.empty();
	}
}