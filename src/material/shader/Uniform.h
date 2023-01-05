#ifndef SKELETALANIMATION_UNIFORM_H
#define SKELETALANIMATION_UNIFORM_H

#include <cstdint>
#include <glm/glm.hpp>

#include "Bone.h"

namespace Animation
{
	class Uniform
	{
	protected:
		static constexpr std::uint32_t UNIFORM_BINDING = 0;
		static constexpr std::uint32_t SAMPLER_BINDING = 1;

	public:
		struct UniformData
		{
			alignas(16) glm::mat4 boneTransforms[Bone::MAX_BONES];
			alignas(16) glm::mat4 modelMatrix;
			alignas(16) glm::mat4 viewMatrix;
			alignas(16) glm::mat4 projectionMatrix;
			alignas(16) glm::vec4 lightColor;
			alignas(16) glm::vec3 cameraPosition;
			alignas(16) glm::vec3 lightPosition;
			alignas(16) glm::vec3 diffuseColor;
			alignas(4) float shininess;
			alignas(4) float specularStrength;
		};

		virtual ~Uniform() = default;

		virtual void bind() const = 0;

		virtual void update(const UniformData &uniformData) = 0;

		virtual void unbind() const = 0;
	};
}

#endif //SKELETALANIMATION_UNIFORM_H
