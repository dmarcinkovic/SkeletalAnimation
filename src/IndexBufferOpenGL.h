#ifndef SKELETALANIMATION_INDEXBUFFEROPENGL_H
#define SKELETALANIMATION_INDEXBUFFEROPENGL_H

#include <GL/glew.h>

#include "IndexBuffer.h"

namespace Animation
{
	class IndexBufferOpenGL : public IndexBuffer
	{
	private:
		GLuint m_Id{};

	public:
		IndexBufferOpenGL();

		~IndexBufferOpenGL() override;

		void bind() override;

		void unbind() override;

		void storeData(const std::vector<std::uint32_t> &indices) override;
	};
}

#endif //SKELETALANIMATION_INDEXBUFFEROPENGL_H
