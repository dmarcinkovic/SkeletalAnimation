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

		void setTexture(const std::unique_ptr<Texture> &texture) override;

	private:
		void attachShaders() const;

		void linkProgram() const;

		void validateProgram() const;

		void createVertexShader();

		void createFragmentShader();

		static GLuint loadShader(const std::string &shaderSource, GLenum type);

		static void debug(GLuint shaderId);

		static class RendererOpenGL *getRenderer();

		static class TextureOpenGL *getTexture(const std::unique_ptr<Texture> &texture);
	};
}

#endif //SKELETALANIMATION_SHADEROPENGL_H
