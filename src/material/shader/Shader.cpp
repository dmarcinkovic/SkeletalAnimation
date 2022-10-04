#include <cassert>

#include "Shader.h"

namespace Animation
{
	void Shader::update()
	{
		for (const auto &uniform: m_Uniforms)
		{
			assert(uniform);
			uniform->update();
		}
	}

	void Shader::addUniform(std::unique_ptr<Uniform> uniform)
	{
		assert(uniform);
		m_Uniforms.emplace_back(std::move(uniform));
	}
}