#ifndef SKELETALANIMATION_SHADER_H
#define SKELETALANIMATION_SHADER_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "Uniform.h"
#include "Texture.h"

namespace Animation
{
	class Shader
	{
	protected:
		std::unique_ptr<Uniform> m_Uniform;

	public:
		virtual ~Shader() = default;

		virtual void startShader() const = 0;

		virtual void stopShader() const = 0;

		virtual void setTexture(const std::unique_ptr<Texture> &texture) = 0;

		void updateUniforms(const Uniform::UniformData &uniformData);
	};
}

#endif //SKELETALANIMATION_SHADER_H
