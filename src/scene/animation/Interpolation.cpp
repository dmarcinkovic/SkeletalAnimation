#include <assimp/scene.h>
#include <glm/gtx/quaternion.hpp>

#include "Interpolation.h"

namespace
{
	float calculateFactor(double animationTimeInTicks, double previousFrameTime, double nextFrameTime)
	{
		double factor = (animationTimeInTicks - previousFrameTime) / (nextFrameTime - previousFrameTime);
		assert(factor >= 0 && factor <= 1);

		return static_cast<float>(factor);
	}

	std::pair<int, int> getScalingPreviousAndNextIndex(float animationTimeInTicks, const aiNodeAnim *nodeAnim)
	{
		assert(nodeAnim->mNumScalingKeys > 0);

		int previousIndex = 0;
		unsigned int nextIndex = nodeAnim->mNumScalingKeys - 1;

		for (int i = 1; i < nodeAnim->mNumScalingKeys; ++i)
		{
			if (nodeAnim->mScalingKeys[i].mTime > animationTimeInTicks)
			{
				nextIndex = i;
				break;
			} else
			{
				previousIndex = i;
			}
		}

		assert(previousIndex >= 0 && previousIndex < nodeAnim->mNumScalingKeys);
		assert(nextIndex >= 0 && nextIndex < nodeAnim->mNumScalingKeys);

		return {previousIndex, nextIndex};
	}

	glm::vec3 getInterpolatedScaling(float animationTimeInTicks, const aiNodeAnim *nodeAnim)
	{
		auto [prev, next] = getScalingPreviousAndNextIndex(animationTimeInTicks, nodeAnim);

		if (prev == next)
		{
			const aiVector3D &scale = nodeAnim->mScalingKeys[prev].mValue;
			return {scale.x, scale.y, scale.z};
		}

		double previousFrameTime = nodeAnim->mScalingKeys[prev].mTime;
		double nextFrameTime = nodeAnim->mScalingKeys[next].mTime;
		double deltaTime = nextFrameTime - previousFrameTime;
		assert(deltaTime > 0);

		float factor = calculateFactor(animationTimeInTicks, previousFrameTime, nextFrameTime);

		const aiVector3D &start = nodeAnim->mScalingKeys[prev].mValue;
		const aiVector3D &end = nodeAnim->mScalingKeys[next].mValue;

		const aiVector3D scale = start + factor * (end - start);
		return {scale.x, scale.y, scale.z};
	}

	std::pair<int, int> getPositionPreviousAndNextIndex(float animationTimeInTicks, const aiNodeAnim *nodeAnim)
	{
		assert(nodeAnim->mNumPositionKeys > 0);

		int previousIndex = 0;
		unsigned int nextIndex = nodeAnim->mNumPositionKeys - 1;

		for (int i = 1; i < nodeAnim->mNumPositionKeys; ++i)
		{
			if (nodeAnim->mPositionKeys[i].mTime > animationTimeInTicks)
			{
				nextIndex = i;
				break;
			} else
			{
				previousIndex = i;
			}
		}

		assert(previousIndex >= 0 && previousIndex < nodeAnim->mNumPositionKeys);
		assert(nextIndex >= 0 && nextIndex < nodeAnim->mNumPositionKeys);

		return {previousIndex, nextIndex};
	}

	glm::vec3 getInterpolatedPosition(float animationTimeInTicks, const aiNodeAnim *nodeAnim)
	{
		auto [prev, next] = getPositionPreviousAndNextIndex(animationTimeInTicks, nodeAnim);

		if (prev == next)
		{
			const aiVector3D &position = nodeAnim->mPositionKeys[prev].mValue;
			return {position.x, position.y, position.z};
		}

		double previousFrameTime = nodeAnim->mPositionKeys[prev].mTime;
		double nextFrameTime = nodeAnim->mPositionKeys[next].mTime;
		double deltaTime = nextFrameTime - previousFrameTime;
		assert(deltaTime > 0);

		float factor = calculateFactor(animationTimeInTicks, previousFrameTime, nextFrameTime);

		const aiVector3D &start = nodeAnim->mPositionKeys[prev].mValue;
		const aiVector3D &end = nodeAnim->mPositionKeys[next].mValue;

		const aiVector3D position = start + factor * (end - start);
		return {position.x, position.y, position.z};
	}

	std::pair<int, int> getRotationPreviousAndNextIndex(float animationTimeInTicks, const aiNodeAnim *nodeAnim)
	{
		assert(nodeAnim->mNumRotationKeys > 0);

		int previousIndex = 0;
		unsigned int nextIndex = nodeAnim->mNumRotationKeys - 1;

		for (int i = 1; i < nodeAnim->mNumRotationKeys; ++i)
		{
			if (nodeAnim->mRotationKeys[i].mTime > animationTimeInTicks)
			{
				nextIndex = i;
				break;
			} else
			{
				previousIndex = i;
			}
		}

		assert(previousIndex >= 0 && previousIndex < nodeAnim->mNumRotationKeys);
		assert(nextIndex >= 0 && nextIndex < nodeAnim->mNumRotationKeys);

		return {previousIndex, nextIndex};
	}

	glm::quat getInterpolatedRotation(float animationTimeInTicks, const aiNodeAnim *nodeAnim)
	{
		auto [prev, next] = getRotationPreviousAndNextIndex(animationTimeInTicks, nodeAnim);

		if (prev == next)
		{
			const aiQuaternion &quaternion = nodeAnim->mRotationKeys[prev].mValue;
			return {quaternion.w, quaternion.x, quaternion.y, quaternion.z};
		}

		double previousFrameTime = nodeAnim->mRotationKeys[prev].mTime;
		double nextFrameTime = nodeAnim->mRotationKeys[next].mTime;
		double deltaTime = nextFrameTime - previousFrameTime;
		assert(deltaTime > 0);

		float factor = calculateFactor(animationTimeInTicks, previousFrameTime, nextFrameTime);

		const aiQuaternion &start = nodeAnim->mRotationKeys[prev].mValue;
		const aiQuaternion &end = nodeAnim->mRotationKeys[next].mValue;

		aiQuaternion quaternion;
		aiQuaternion::Interpolate(quaternion, start, end, factor);

		return {quaternion.w, quaternion.x, quaternion.y, quaternion.z};
	}
}

namespace Animation
{
	glm::mat4 Interpolation::getTransform(float animationTimeInTicks, const aiNodeAnim *nodeAnim)
	{
		glm::vec3 position = getInterpolatedPosition(animationTimeInTicks, nodeAnim);
		glm::vec3 scale = getInterpolatedScaling(animationTimeInTicks, nodeAnim);
		glm::quat rotation = getInterpolatedRotation(animationTimeInTicks, nodeAnim);

		glm::mat4 translationMatrix = glm::translate(glm::mat4{1.0f}, position);
		glm::mat4 rotationMatrix = glm::toMat4(rotation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4{1.0f}, scale);

		return translationMatrix * rotationMatrix * scaleMatrix;
	}
}