#ifndef SKELETALANIMATION_TEXTURE_H
#define SKELETALANIMATION_TEXTURE_H

#include <filesystem>

namespace Animation
{
	class Texture
	{
	protected:
		int m_Width{};
		int m_Height{};

		std::uint8_t *m_Pixels{};

	public:
		explicit Texture(const std::filesystem::path &path);

		Texture(const std::uint8_t *data, int size);

		Texture(int width, int height, const std::uint8_t *pixels);

		Texture(const Texture &) = delete;

		Texture(Texture &&) noexcept = delete;

		virtual ~Texture();
	};
}

#endif //SKELETALANIMATION_TEXTURE_H
