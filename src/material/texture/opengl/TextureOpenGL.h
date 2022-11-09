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

		void createTexture(const std::unique_ptr<UniformOpenGL> &uniform);

	private:
		static void setTextureParameters();
	};
}

#endif //SKELETALANIMATION_TEXTUREOPENGL_H
