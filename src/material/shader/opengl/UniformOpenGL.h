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
		explicit UniformOpenGL(GLuint programId);

		~UniformOpenGL() override = default;

		void bind() const override;

		void update(const UniformData &uniformData) override;

		void unbind() const override;

		void bindSampler() const;

		static void unbindSampler();

	private:
		void createBuffer();

		void allocateBuffer(GLuint programId) const;

		void createSampler(GLuint programId);
	};
}

#endif //SKELETALANIMATION_UNIFORMOPENGL_H
