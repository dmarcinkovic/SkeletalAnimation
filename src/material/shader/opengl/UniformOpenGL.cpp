#include <GL/glew.h>
#include <cassert>

#include "UniformOpenGL.h"

namespace Animation
{
	UniformOpenGL::UniformOpenGL(GLuint programId)
	{
		assert(programId > 0);
		createBuffer();
		allocateBuffer(programId);
		createSampler(programId);
	}

	void UniformOpenGL::update(const UniformData &uniformData)
	{
		static constexpr int TEXTURE_UNIT = 0;

		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UniformData), &uniformData);

		glUniform1i(m_TextureLocation, TEXTURE_UNIT);
		glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT);
	}

	void UniformOpenGL::bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_UniformId);

		bindSampler();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void UniformOpenGL::unbind() const
	{
		glDisable(GL_BLEND);
		unbindSampler();

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void UniformOpenGL::createBuffer()
	{
		glGenBuffers(1, &m_UniformId);
		assert(m_UniformId > 0);
	}

	void UniformOpenGL::allocateBuffer(GLuint programId) const
	{
		bind();
		glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformData), nullptr, GL_STATIC_DRAW);

		GLuint uniformIndex = glGetUniformBlockIndex(programId, "UniformBufferObject");
		assert(uniformIndex != GL_INVALID_INDEX);

		glUniformBlockBinding(programId, uniformIndex, UNIFORM_BINDING);
		glBindBufferRange(GL_UNIFORM_BUFFER, UNIFORM_BINDING, m_UniformId, 0, sizeof(UniformData));

		unbind();
	}

	void UniformOpenGL::createSampler(GLuint programId)
	{
		m_TextureLocation = glGetUniformLocation(programId, "texSampler");
		assert(m_TextureLocation >= 0);

		glGenTextures(1, &m_TextureId);
		assert(m_TextureId > 0);
	}

	void UniformOpenGL::bindSampler() const
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureId);
	}

	void UniformOpenGL::unbindSampler()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}