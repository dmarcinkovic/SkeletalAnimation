#include <cassert>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

namespace Animation
{
	void Shader::update(float shininess, float specularStrength)
	{
		assert(m_Uniform);

		const glm::vec4 lightColor{1, 1, 1, 1};
		const glm::vec3 cameraPosition{0, 0, 1};
		const glm::vec3 lightPosition{1, 1, 1};
		const glm::vec3 translation{0, 0, -2};
		const glm::vec3 scale{1.0f};
		const glm::quat rotation = glm::quat(glm::radians(glm::vec3{0, 45, 0}));
		const glm::mat4 modelMatrix = glm::translate(glm::mat4{1.0f}, translation) * glm::toMat4(rotation) *
									  glm::scale(glm::mat4{1.0f}, scale);
		const glm::mat4 viewMatrix = glm::lookAt(cameraPosition, glm::vec3{0}, glm::vec3{0, 1, 0});
		const glm::mat4 projectionMatrix = getProjectionMatrix();

		Uniform::UniformData uniformData{};
		uniformData.modelMatrix = modelMatrix;
		uniformData.viewMatrix = viewMatrix;
		uniformData.projectionMatrix = projectionMatrix;
		uniformData.cameraPosition = cameraPosition;
		uniformData.lightPosition = lightPosition;
		uniformData.lightColor = lightColor;
		uniformData.shininess = shininess;
		uniformData.specularStrength = specularStrength;

		m_Uniform->update(uniformData);
	}
}