#include <cassert>
#include <spdlog/spdlog.h>

#include "ShaderOpenGL.h"
#include "StandardVertexShader.glsl.h"
#include "StandardFragmentShader.glsl.h"

namespace Animation
{
	ShaderOpenGL::ShaderOpenGL()
	{
		createVertexShader();
		createFragmentShader();

		m_Program = glCreateProgram();
		assert(m_Program > 0);

		attachShaders();
		linkProgram();
		validateProgram();
	}

	ShaderOpenGL::~ShaderOpenGL()
	{
		assert(m_Program > 0);
		assert(m_VertexShader > 0);
		assert(m_FragmentShader > 0);

		glDetachShader(m_Program, m_VertexShader);
		glDetachShader(m_Program, m_FragmentShader);

		glDeleteShader(m_VertexShader);
		glDeleteShader(m_FragmentShader);
		glDeleteProgram(m_Program);
	}

	void ShaderOpenGL::startShader()
	{
		assert(m_Program > 0);
		glUseProgram(m_Program);
	}

	void ShaderOpenGL::stopShader()
	{
		glUseProgram(0);
	}

	void ShaderOpenGL::attachShaders() const
	{
		glAttachShader(m_Program, m_VertexShader);
		glAttachShader(m_Program, m_FragmentShader);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			spdlog::error("Failed to attach shader. Error code: '{}'.", error);
			std::exit(EXIT_FAILURE);
		}
	}

	void ShaderOpenGL::validateProgram() const
	{
		glValidateProgram(m_Program);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			spdlog::error("Failed to validate shader program. Error code: '{}'.", error);
			std::exit(EXIT_FAILURE);
		}
	}

	void ShaderOpenGL::linkProgram() const
	{
		glLinkProgram(m_Program);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			spdlog::error("Failed to link shader program. Error code: '{}'.", error);
			std::exit(EXIT_FAILURE);
		}
	}

	void ShaderOpenGL::createVertexShader()
	{
		unsigned char *vertexData = StandardVertexShader_glsl;
		std::string vertexShaderSource(vertexData, vertexData + StandardVertexShader_glsl_len);

		m_VertexShader = loadShader(vertexShaderSource, GL_VERTEX_SHADER);
		assert(m_VertexShader > 0);
	}

	void ShaderOpenGL::createFragmentShader()
	{
		unsigned char *fragmentData = StandardFragmentShader_glsl;
		std::string fragmentShaderSource(fragmentData, fragmentData + StandardFragmentShader_glsl_len);

		m_FragmentShader = loadShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
		assert(m_FragmentShader > 0);
	}

	GLuint ShaderOpenGL::loadShader(const std::string &shaderSource, GLenum type)
	{
		GLuint shader = glCreateShader(type);
		assert(shader > 0);

		const char *source = shaderSource.c_str();
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		debug(shader);

		return shader;
	}

	void ShaderOpenGL::debug(GLuint shaderId)
	{
		int result;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
			char *message = static_cast<char *>(alloca(length * sizeof(char)));

			glGetShaderInfoLog(shaderId, length, &length, message);
			spdlog::error("Error while compiling shader: '{}',", message);

			glDeleteShader(shaderId);
			std::exit(EXIT_FAILURE);
		}
	}
}