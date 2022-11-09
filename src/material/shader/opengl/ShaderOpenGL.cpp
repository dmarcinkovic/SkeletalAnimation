#include <cassert>
#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderOpenGL.h"
#include "UniformOpenGL.h"
#include "VertexShader.glsl.h"
#include "FragmentShader.glsl.h"
#include "TextureOpenGL.h"
#include "RendererOpenGL.h"

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

	void ShaderOpenGL::startShader() const
	{
		assert(m_Program > 0);
		glUseProgram(m_Program);

		assert(m_Uniform);
		m_Uniform->bind();
	}

	void ShaderOpenGL::stopShader() const
	{
		m_Uniform->unbind();
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
		unsigned char *vertexData = vertexShader_glsl;
		std::string vertexShaderSource(vertexData, vertexData + vertexShader_glsl_len);

		m_VertexShader = loadShader(vertexShaderSource, GL_VERTEX_SHADER);
		assert(m_VertexShader > 0);
	}

	void ShaderOpenGL::createFragmentShader()
	{
		unsigned char *fragmentData = fragmentShader_glsl;
		std::string fragmentShaderSource(fragmentData, fragmentData + fragmentShader_glsl_len);

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

	void ShaderOpenGL::setTexture(const std::unique_ptr<Texture> &texture)
	{
		TextureOpenGL *textureOpenGL = getTexture(texture);

		std::unique_ptr<UniformOpenGL> uniform = std::make_unique<UniformOpenGL>(m_Program);
		textureOpenGL->createTexture(uniform);

		m_Uniform = std::move(uniform);
	}

	glm::mat4 ShaderOpenGL::getProjectionMatrix() const
	{
		RendererOpenGL *renderer = getRenderer();

		const float aspect = renderer->getWindowAspectRatio();
		return glm::perspective(glm::radians(FOV), aspect, NEAR, FAR);
	}

	RendererOpenGL *ShaderOpenGL::getRenderer()
	{
		std::unique_ptr<Renderer> &renderer = Renderer::getRenderer();
		assert(renderer);

		auto *openGLRenderer = dynamic_cast<RendererOpenGL *>(renderer.get());
		assert(openGLRenderer);

		return openGLRenderer;
	}

	class TextureOpenGL *ShaderOpenGL::getTexture(const std::unique_ptr<Texture> &texture)
	{
		assert(texture);
		auto *textureOpenGL = dynamic_cast<TextureOpenGL *>(texture.get());
		assert(textureOpenGL);

		return textureOpenGL;
	}
}