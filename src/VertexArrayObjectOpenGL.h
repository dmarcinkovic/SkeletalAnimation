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

		void bind() override;

		void unbind() override;

		void storeData(int attributeIndex, std::unique_ptr<VertexBufferObject> vertexBufferObject) override;
	};
}

#endif //SKELETALANIMATION_VERTEXARRAYOBJECTOPENGL_H
