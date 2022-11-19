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
		static constexpr float FOV = 45.0f;
		static constexpr float NEAR = 0.1f;
		static constexpr float FAR = 100.0f;

		std::unique_ptr<Uniform> m_Uniform;

	public:
		virtual ~Shader() = default;

		virtual void startShader() const = 0;

		virtual void stopShader() const = 0;

		virtual void setTexture(const std::unique_ptr<Texture> &texture) = 0;

		[[nodiscard]] virtual glm::mat4 getProjectionMatrix() const = 0;

		void update(const class Material *material);
	};
}

#endif //SKELETALANIMATION_SHADER_H
