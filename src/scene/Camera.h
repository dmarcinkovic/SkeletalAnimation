#ifndef SKELETALANIMATION_CAMERA_H
#define SKELETALANIMATION_CAMERA_H

#include <glm/glm.hpp>

namespace Animation
{
	class Camera final
	{
	private:
		float m_Fov{45.0f};
		float m_Near{0.1f};
		float m_Far{100.0f};

		glm::vec3 m_Position{0, 0, 1};
		glm::vec3 m_Center{0};
		glm::vec3 m_Up{0, 1, 0};

	public:
		Camera() = default;

		explicit Camera(const class aiCamera *camera);

		[[nodiscard]] glm::mat4 getViewMatrix() const;
	};
}

#endif //SKELETALANIMATION_CAMERA_H
