#ifndef SKELETALANIMATION_LIGHT_H
#define SKELETALANIMATION_LIGHT_H

#include <glm/glm.hpp>

namespace Animation
{
	class Light final
	{
	private:
		glm::vec3 m_Position{1, 0, 0};
		glm::vec4 m_Color{1, 1, 1, 1};

	public:
		Light() = default;

		explicit Light(const class aiLight *light);

		[[nodiscard]] glm::vec3 getPosition() const;

		[[nodiscard]] glm::vec4 getColor() const;
	};
}

#endif //SKELETALANIMATION_LIGHT_H
