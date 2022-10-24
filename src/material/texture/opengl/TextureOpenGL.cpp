#include "TextureOpenGL.h"

namespace Animation
{
	// TODO: do not hardcode this values: pass program id as an argument
	// TODO: store binding as a constant in Shader.h
	TextureOpenGL::TextureOpenGL(const std::filesystem::path &path)
			: Texture(path)
	{
	}

	void TextureOpenGL::setTextureParameters()
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void TextureOpenGL::createTexture(const std::unique_ptr<UniformOpenGL> &uniform)
	{
		uniform->bindSampler();
		setTextureParameters();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Pixels);
		UniformOpenGL::unbindSampler();
	}
}