#ifndef SKELETALANIMATION_UNIFORM_H
#define SKELETALANIMATION_UNIFORM_H

#include <cstdint>

namespace Animation
{
	class Uniform
	{
	protected:
		std::uint32_t m_UniformBinding;
		std::uint32_t m_SamplerBinding;

	public:
		// TODO: create struct that will define uniform layout in shader
		explicit Uniform(std::uint32_t uniformBinding, std::uint32_t samplerBinding);

		virtual ~Uniform() = default;

		virtual void bind() const = 0;

		virtual void update() = 0;

		virtual void unbind() const = 0;
	};
}

#endif //SKELETALANIMATION_UNIFORM_H
