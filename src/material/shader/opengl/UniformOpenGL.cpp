#include <GL/glew.h>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cassert>

#include "UniformOpenGL.h"

namespace Animation
{
	UniformOpenGL::UniformOpenGL(std::uint32_t binding, GLuint programId)
			: Uniform(binding)
	{
		assert(programId > 0);
		createBuffer();
		allocateBuffer(programId);
	}

	void UniformOpenGL::update()
	{
		glm::vec4 color{0.4f, 0.4f, 1.0f, 1};
		bind();
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(color), glm::value_ptr(color));
		unbind();
	}

	void UniformOpenGL::bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_UniformId);
	}

	void UniformOpenGL::unbind()
	{
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

		glUniformBlockBinding(programId, uniformIndex, m_Binding);
		glBindBufferRange(GL_UNIFORM_BUFFER, m_Binding, m_UniformId, 0, sizeof(glm::vec4));

		unbind();
	}
}