#ifndef SKELETALANIMATION_VERTEXARRAYOBJECTOPENGL_H
#define SKELETALANIMATION_VERTEXARRAYOBJECTOPENGL_H

#include <GL/glew.h>

#include "VertexArrayObject.h"

namespace Animation
{
	class VertexArrayObjectOpenGL final : public VertexArrayObject
	{
	private:
		GLuint m_Id{};

	public:
		VertexArrayObjectOpenGL();

		~VertexArrayObjectOpenGL() override;

		void bind() const override;

		void unbind() const override;

		void storeFloatData(int attributeIndex, std::unique_ptr<VertexBufferObject> vertexBuffer) override;

		void storeIntData(int attributeIndex, std::unique_ptr<VertexBufferObject> vertexBuffer) override;

		void setIndexBufferData(const std::vector<std::uint32_t> &indices) override;
	};
}

#endif //SKELETALANIMATION_VERTEXARRAYOBJECTOPENGL_H
