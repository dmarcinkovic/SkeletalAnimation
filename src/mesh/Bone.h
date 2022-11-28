#ifndef SKELETALANIMATION_BONE_H
#define SKELETALANIMATION_BONE_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace Animation
{
	class Bone final
	{
	private:
		std::string m_Name;
		glm::mat4 m_BoneTransform{1.0f};

		std::vector<Bone> m_Children;

	public:

	};
}

#endif //SKELETALANIMATION_BONE_H
