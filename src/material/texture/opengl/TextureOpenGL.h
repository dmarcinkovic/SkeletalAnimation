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

		// TODO: make make this function virtual: check after you do vulkan
		void createTexture(const std::unique_ptr<UniformOpenGL> &uniform);

	private:
		static void setTextureParameters();
	};
}

#endif //SKELETALANIMATION_TEXTUREOPENGL_H
