#include <cassert>

#include "Material.h"

namespace Animation
{
	Material::Material(std::unique_ptr<Shader> shader, std::unique_ptr<Texture> texture)
			: m_Shader(std::move(shader)), m_Texture(std::move(texture))
	{
		assert(m_Shader);
		assert(m_Texture);

		m_Shader->setTexture(m_Texture);
	}

	void Material::start() const
	{
		m_Shader->startShader();
	}

	void Material::update() const
	{
		m_Shader->update(m_Shininess, m_SpecularStrength);
	}

	void Material::end() const
	{
		m_Shader->stopShader();
	}
}