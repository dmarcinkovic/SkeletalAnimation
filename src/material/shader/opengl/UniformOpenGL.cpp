#include <GL/glew.h>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cassert>

#include "UniformOpenGL.h"

namespace Animation
{
	UniformOpenGL::UniformOpenGL(std::uint32_t uniformBinding, std::uint32_t samplerBinding, GLuint programId)
			: Uniform(uniformBinding, samplerBinding)
	{
		assert(programId > 0);
		createBuffer();
		allocateBuffer(programId);
		createSampler(programId);
	}

	void UniformOpenGL::update()
	{
		static constexpr int TEXTURE_UNIT = 0;
		glm::vec4 color{0.4f, 0.4f, 1.0f, 1};

		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(color), glm::value_ptr(color));

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

	void UniformOpenGL::allocateBuffer(GLuint programId)
	{
		bind();
		// TODO: change type of uniform
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);

		GLuint uniformIndex = glGetUniformBlockIndex(programId, "UniformBufferObject");
		assert(uniformIndex != GL_INVALID_INDEX);

		glUniformBlockBinding(programId, uniformIndex, m_UniformBinding);
		glBindBufferRange(GL_UNIFORM_BUFFER, m_UniformBinding, m_UniformId, 0, sizeof(glm::vec4));

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