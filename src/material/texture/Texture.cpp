#include <stb_image.h>
#include <cassert>

#include "Texture.h"

namespace Animation
{
	Texture::Texture(const std::filesystem::path &path)
	{
		assert(std::filesystem::exists(path));

		stbi_set_flip_vertically_on_load(true);

		int numberOfChannels;
		m_Pixels = stbi_load(path.c_str(), &m_Width, &m_Height, &numberOfChannels, STBI_rgb_alpha);
		assert(m_Pixels);
		assert(m_Width > 0);
		assert(m_Height > 0);
	}

	Texture::~Texture()
	{
		stbi_image_free(m_Pixels);
	}
}