#ifndef SKELETALANIMATION_UNIFORM_H
#define SKELETALANIMATION_UNIFORM_H

#include <cstdint>

namespace Animation
{
	class Uniform
	{
	protected:
		std::uint32_t m_Binding{};

	public:
		// TODO: create struct that will define uniform layout in shader
		explicit Uniform(std::uint32_t binding);

		virtual ~Uniform() = default;

		virtual void update() = 0;
	};
}

#endif //SKELETALANIMATION_UNIFORM_H
