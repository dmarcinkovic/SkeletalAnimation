#include <cassert>

#include "Material.h"

namespace Animation
{
	Material::Material(std::unique_ptr<Shader> shader)
			: m_Shader(std::move(shader))
	{
		assert(m_Shader);
	}

	const std::unique_ptr<Shader> &Material::getShader() const
	{
		assert(m_Shader);
		return m_Shader;
	}
}