#include <assimp/scene.h>
#include <cassert>
#include <stack>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include "AnimationClip.h"
#include "Bone.h"
#include "Interpolation.h"

namespace
{
	glm::mat4 toMat4(const aiMatrix4x4 &matrix)
	{
		return glm::transpose(glm::make_mat4(&matrix.a1));
	}
}

namespace Animation
{
	AnimationClip::AnimationClip(const aiAnimation *animation)
			: m_TicksPerSecond(static_cast<float>(animation->mTicksPerSecond)),
			  m_DurationInTicks(static_cast<float>(animation->mDuration))
	{
		if (m_TicksPerSecond == 0)
		{
			m_TicksPerSecond = 25.0f;
		}

		float durationInSeconds = m_DurationInTicks / m_TicksPerSecond;
		spdlog::info("Animation '{}' is {} seconds long.", animation->mName.C_Str(), durationInSeconds);

		for (int i = 0; i < animation->mNumChannels; ++i)
		{
			const aiNodeAnim *animatedNode = animation->mChannels[i];
			assert(animatedNode);

			m_AnimatedNodes[animatedNode->mNodeName.C_Str()] = animatedNode;
		}
	}

	void AnimationClip::play(const aiNode *rootNode, float deltaTimeInSeconds)
	{
		glm::mat4 globalInverseTransform = glm::inverse(toMat4(rootNode->mTransformation));

		std::stack<const aiNode *> nodes;
		std::stack<glm::mat4> nodeTransforms;

		nodes.push(rootNode);
		nodeTransforms.emplace(1.0f);

		m_Transforms = std::vector<glm::mat4>(Bone::numberOfBones());

		while (!nodes.empty())
		{
			const aiNode *node = nodes.top();
			nodes.pop();

			glm::mat4 parentTransform = nodeTransforms.top();
			nodeTransforms.pop();

			glm::mat4 nodeTransform = getNodeTransform(node, parentTransform);

			for (int i = 0; i < node->mNumChildren; ++i)
			{
				const aiNode *child = node->mChildren[i];
				assert(child);

				nodes.push(child);
				nodeTransforms.push(nodeTransform);
			}

			const std::string nodeName = node->mName.C_Str();
			if (Bone::isValidBone(nodeName))
			{
				const aiMatrix4x4 &offsetMatrix = Bone::getBoneOffset(nodeName);
				glm::mat4 offset = toMat4(offsetMatrix);

				int boneIndex = Bone::getBoneIndex(nodeName);
				m_Transforms[boneIndex] = globalInverseTransform * nodeTransform * offset;
			}
		}

		updateAnimationTime(deltaTimeInSeconds);
	}

	glm::mat4 AnimationClip::getNodeTransform(const aiNode *node, const glm::mat4 &parentTransform)
	{
		const std::string nodeName = node->mName.C_Str();
		if (Bone::isValidBone(nodeName))
		{
			glm::mat4 nodeTransformation;

			if (m_AnimatedNodes.contains(nodeName))
			{
				const aiNodeAnim *animatedNode = m_AnimatedNodes[nodeName];
				nodeTransformation = Interpolation::getTransform(m_AnimationTimeInTicks, animatedNode);
			} else
			{
				const aiMatrix4x4 transformation = node->mTransformation;
				nodeTransformation = toMat4(transformation);
			}

			return parentTransform * nodeTransformation;
		}

		return glm::mat4{1.0f};
	}

	void AnimationClip::updateAnimationTime(float deltaTimeInSeconds)
	{
		float timeInTicks = deltaTimeInSeconds * m_TicksPerSecond;

		m_AnimationTimeInTicks += timeInTicks;
		m_AnimationDone = (m_AnimationTimeInTicks >= m_DurationInTicks);

		if (m_AnimationDone)
		{
			m_AnimationTimeInTicks = 0.0f;
		}
	}

	const std::vector<glm::mat4> &AnimationClip::getTransforms() const
	{
		return m_Transforms;
	}

	bool AnimationClip::isDone() const
	{
		return m_AnimationDone;
	}
}