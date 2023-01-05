#include <assimp/scene.h>
#include <glm/ext/matrix_transform.hpp>

#include "Camera.h"

namespace
{
	glm::vec3 toVec3(const aiVector3D &vector)
	{
		return {vector.x, vector.y, vector.z};
	}
}

namespace Animation
{
	Camera::Camera(const aiCamera *camera)
			: m_Fov(camera->mHorizontalFOV), m_Near(camera->mClipPlaneNear), m_Far(camera->mClipPlaneFar),
			  m_Position(toVec3(camera->mPosition)), m_Center(toVec3(camera->mLookAt)),
			  m_Up(toVec3(camera->mUp))
	{
	}

	const glm::vec3 &Camera::getPosition() const
	{
		return m_Position;
	}

	glm::mat4 Camera::getViewMatrix() const
	{
		return glm::lookAt(m_Position, m_Center, m_Up);
	}

	float Camera::getFov() const
	{
		return m_Fov;
	}

	float Camera::getNear() const
	{
		return m_Near;
	}

	float Camera::getFar() const
	{
		return m_Far;
	}
}