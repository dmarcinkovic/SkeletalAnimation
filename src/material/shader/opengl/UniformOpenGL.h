#ifndef SKELETALANIMATION_UNIFORMOPENGL_H
#define SKELETALANIMATION_UNIFORMOPENGL_H

#include "Uniform.h"

namespace Animation
{
	class UniformOpenGL final : public Uniform
	{
	private:
		GLuint m_UniformId{};

	public:
		UniformOpenGL(std::uint32_t binding, GLuint programId);

		~UniformOpenGL() override = default;

		void update() override;

	private:
		void createBuffer();

		void allocateBuffer(GLuint programId);

		void bind() const;

		static void unbind();
	};
}

#endif //SKELETALANIMATION_UNIFORMOPENGL_H
