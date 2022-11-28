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

		void storeFloatData(std::uint32_t binding, const std::vector<float> &data, int dataSize) override;

		void storeIntData(std::uint32_t binding, const std::vector<int> &data, int dataSize) override;

	private:
		void storeData(const void *data, GLsizeiptr bufferSize) const;
	};
}

#endif //SKELETALANIMATION_VERTEXBUFFEROBJECTOPENGL_H
