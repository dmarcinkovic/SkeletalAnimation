#ifndef SKELETALANIMATION_SHADEROPENGL_H
#define SKELETALANIMATION_SHADEROPENGL_H

#include <GL/glew.h>

#include "Shader.h"

namespace Animation
{
	class ShaderOpenGL final : public Shader
	{
	private:
		GLuint m_VertexShader{};
		GLuint m_FragmentShader{};
		GLuint m_Program;

	public:
		ShaderOpenGL();

		~ShaderOpenGL() override;

		void startShader() const override;

		void stopShader() const override;

	private:
		void attachShaders() const;

		void linkProgram() const;

		void validateProgram() const;

		void createVertexShader();

		void createFragmentShader();

		static GLuint loadShader(const std::string &shaderSource, GLenum type);

		static void debug(GLuint shaderId);

		void setUniforms();
	};
}

#endif //SKELETALANIMATION_SHADEROPENGL_H
