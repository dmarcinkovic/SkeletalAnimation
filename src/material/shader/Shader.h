#ifndef SKELETALANIMATION_SHADER_H
#define SKELETALANIMATION_SHADER_H

#include <vector>
#include <memory>

#include "Uniform.h"

namespace Animation
{
	class Shader
	{
	protected:
		std::vector<std::unique_ptr<Uniform>> m_Uniforms;

	public:
		virtual ~Shader() = default;

		virtual void startShader() const = 0;

		virtual void stopShader() const = 0;

		void update();

	protected:
		void addUniform(std::unique_ptr<Uniform> uniform);
	};
}

#endif //SKELETALANIMATION_SHADER_H
