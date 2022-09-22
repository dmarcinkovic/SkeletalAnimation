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

		void storeData(int attributeIndex, std::unique_ptr<VertexBufferObject> vertexBufferObject) override;

		void setIndexBufferData(const std::vector<std::uint32_t> &indices) override;
	};
}

#endif //SKELETALANIMATION_VERTEXARRAYOBJECTOPENGL_H
