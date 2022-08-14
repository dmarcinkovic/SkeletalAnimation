#ifndef SKELETALANIMATION_VERTEXBUFFEROBJECTOPENGL_H
#define SKELETALANIMATION_VERTEXBUFFEROBJECTOPENGL_H

#include <GL/glew.h>

#include "VertexBufferObject.h"

namespace Animation
{
	class VertexBufferObjectOpenGL final : public VertexBufferObject
	{
	private:
		GLuint m_Id{};

	public:
		VertexBufferObjectOpenGL();

		~VertexBufferObjectOpenGL() override;

		void bind() const override;

		void unbind() const override;

		void storeFloatData(const std::vector<float> &data, int dataSize) override;
	};
}

#endif //SKELETALANIMATION_VERTEXBUFFEROBJECTOPENGL_H
