#ifndef SKELETALANIMATION_BONE_H
#define SKELETALANIMATION_BONE_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace Animation
{
	class Bone final
	{
	public:
		Bone() = delete;

		static int getBoneIndex(const std::string &name);

		static void mapBone(const std::string &name);

		static void setBoneOffset(const std::string &name, const aiMatrix4x4 &offset);

		static const aiMatrix4x4 &getBoneOffset(const std::string &name);

		static bool isValidBone(const std::string &name);

		static std::size_t numberOfBones();
	};
}

#endif //SKELETALANIMATION_BONE_H
