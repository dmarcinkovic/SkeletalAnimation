#include <cassert>

#include "Shader.h"

namespace Animation
{
	void Shader::updateUniforms(const Uniform::UniformData &uniformData)
	{
		assert(m_Uniform);
		m_Uniform->update(uniformData);
	}
}