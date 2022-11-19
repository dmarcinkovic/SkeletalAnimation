#include "TextureOpenGL.h"

namespace Animation
{
	TextureOpenGL::TextureOpenGL(const std::filesystem::path &path)
			: Texture(path)
	{
	}

	TextureOpenGL::TextureOpenGL(const std::uint8_t *data, int len)
			: Texture(data, len)
	{
	}

	TextureOpenGL::TextureOpenGL(int width, int height, const std::uint8_t *pixels)
			: Texture(width, height, pixels)
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