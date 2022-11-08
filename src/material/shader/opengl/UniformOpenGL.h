#ifndef SKELETALANIMATION_UNIFORMOPENGL_H
#define SKELETALANIMATION_UNIFORMOPENGL_H

#include "Uniform.h"

namespace Animation
{
	class UniformOpenGL final : public Uniform
	{
	private:
		GLuint m_UniformId{};
		GLuint m_TextureId{};
		GLint m_TextureLocation{};

	public:
		UniformOpenGL(std::uint32_t uniformBinding, std::uint32_t samplerBinding, GLuint programId);

		~UniformOpenGL() override = default;

		void bind() const override;

		void update(const UniformData &uniformData) override;

		void unbind() const override;

		void bindSampler() const;

		static void unbindSampler();

	private:
		void createBuffer();

		void allocateBuffer(GLuint programId);

		void createSampler(GLuint programId);
	};
}

#endif //SKELETALANIMATION_UNIFORMOPENGL_H
