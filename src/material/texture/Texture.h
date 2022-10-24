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

		Texture(const Texture &) = delete;

		Texture(Texture &&) noexcept = delete;

		virtual ~Texture();
	};
}

#endif //SKELETALANIMATION_TEXTURE_H
