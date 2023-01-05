#include <assimp/scene.h>

#include "Light.h"

namespace
{
	glm::vec3 toVec3(const aiVector3D &vector)
	{
		return {vector.x, vector.y, vector.z};
	}

	glm::vec4 toColor(const aiColor3D &color)
	{
		return {color.r, color.g, color.b, 1.0f};
	}
}

namespace Animation
{
	Light::Light(const aiLight *light)
		: m_Position(toVec3(light->mPosition)), m_Color(toColor(light->mColorDiffuse))
	{
	}

	glm::vec3 Light::getPosition() const
	{
		return m_Position;
	}

	glm::vec4 Light::getColor() const
	{
		return m_Color;
	}
}