#ifndef SKELETALANIMATION_UNIFORM_H
#define SKELETALANIMATION_UNIFORM_H

#include <cstdint>
#include <glm/glm.hpp>

namespace Animation
{
	class Uniform
	{
	protected:
		std::uint32_t m_UniformBinding;
		std::uint32_t m_SamplerBinding;

	public:
		struct UniformData
		{
			alignas(16) glm::mat4 modelMatrix;
			alignas(16) glm::mat4 viewMatrix;
			alignas(16) glm::mat4 projectionMatrix;
			alignas(16) glm::vec4 lightColor;
			alignas(16) glm::vec3 cameraPosition;
			alignas(16) glm::vec3 lightPosition;
		};

		explicit Uniform(std::uint32_t uniformBinding, std::uint32_t samplerBinding);

		virtual ~Uniform() = default;

		virtual void bind() const = 0;

		virtual void update(const UniformData &uniformData) = 0;

		virtual void unbind() const = 0;
	};
}

#endif //SKELETALANIMATION_UNIFORM_H
