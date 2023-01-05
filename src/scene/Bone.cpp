#include <spdlog/spdlog.h>

#include "Bone.h"

namespace
{
	std::unordered_map<std::string, int> bones;
	std::unordered_map<std::string, aiMatrix4x4> boneOffsets;
}

namespace Animation
{
	int Bone::getBoneIndex(const std::string &name)
	{
		if (bones.contains(name))
		{
			return bones[name];
		}

		spdlog::error("Bone '{}' does not exists.", name);
		return -1;
	}

	void Bone::mapBone(const std::string &name)
	{
		if (bones.size() >= MAX_BONES)
		{
			spdlog::error("Cannot map bone '{}' because the limit of {} bones is exceeded.", name, MAX_BONES);
		} else if (!bones.contains(name))
		{
			bones[name] = static_cast<int>(bones.size());
		}
	}

	void Bone::setBoneOffset(const std::string &name, const aiMatrix4x4 &offset)
	{
		if (boneOffsets.size() < MAX_BONES)
		{
			assert(bones.contains(name));
			boneOffsets[name] = offset;
		}
	}

	bool Bone::isValidBone(const std::string &name)
	{
		return bones.contains(name);
	}

	const aiMatrix4x4 &Bone::getBoneOffset(const std::string &name)
	{
		assert(boneOffsets.contains(name));
		return boneOffsets[name];
	}

	std::size_t Bone::numberOfBones()
	{
		return bones.size();
	}
}