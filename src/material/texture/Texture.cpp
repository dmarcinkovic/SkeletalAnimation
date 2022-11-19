#include <stb_image.h>
#include <cassert>
#include <cstring>

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

	Texture::Texture(const std::uint8_t *data, int len)
	{
		stbi_set_flip_vertically_on_load(true);

		int numberOfChannels;

		m_Pixels = stbi_load_from_memory(data, len, &m_Width, &m_Height, &numberOfChannels, STBI_rgb_alpha);
		assert(m_Pixels);
		assert(m_Width > 0);
		assert(m_Height > 0);
	}

	Texture::Texture(int width, int height, const std::uint8_t *pixels)
			: m_Width(width), m_Height(height)
	{
		std::memcpy(m_Pixels, pixels, m_Width * m_Height);

		assert(m_Pixels);
		assert(m_Width > 0);
		assert(m_Height > 0);
	}

	Texture::~Texture()
	{
		stbi_image_free(m_Pixels);
	}
}