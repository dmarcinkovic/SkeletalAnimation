#ifndef SKELETALANIMATION_MATERIAL_H
#define SKELETALANIMATION_MATERIAL_H

#include <memory>

#include "Shader.h"
#include "Texture.h"

namespace Animation
{
	class Material final
	{
	private:
		float m_Shininess{32.0f};
		float m_SpecularStrength{0.5f};

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

	public:
		Material(std::unique_ptr<Shader> shader, std::unique_ptr<Texture> texture);

		void start() const;

		void update() const;

		void end() const;
	};
}

#endif //SKELETALANIMATION_MATERIAL_H
