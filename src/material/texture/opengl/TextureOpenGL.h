#ifndef SKELETALANIMATION_TEXTUREOPENGL_H
#define SKELETALANIMATION_TEXTUREOPENGL_H

#include <GL/glew.h>

#include "Texture.h"
#include "UniformOpenGL.h"

namespace Animation
{
	class TextureOpenGL final : public Texture
	{
	public:
		explicit TextureOpenGL(const std::filesystem::path &path);

		TextureOpenGL(const std::uint8_t *data, int len);

		TextureOpenGL(int width, int height, const std::uint8_t *pixels);

		void createTexture(const std::unique_ptr<UniformOpenGL> &uniform);

	private:
		static void setTextureParameters();
	};
}

#endif //SKELETALANIMATION_TEXTUREOPENGL_H
