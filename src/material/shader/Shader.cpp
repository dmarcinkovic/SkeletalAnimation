#include <cassert>

#include "Shader.h"

namespace Animation
{
	void Shader::update()
	{
		assert(m_Uniform);
		m_Uniform->update();
	}

	void Shader::addUniform(std::unique_ptr<Uniform> uniform)
	{
		assert(uniform);
		m_Uniform = std::move(uniform);
	}
}