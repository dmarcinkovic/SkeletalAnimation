#ifndef SKELETALANIMATION_BUFFER_H
#define SKELETALANIMATION_BUFFER_H

namespace Animation
{
	class Buffer
	{
	public:
		virtual ~Buffer() = default;

		virtual void bind() = 0;

		virtual void unbind() = 0;
	};
}

#endif //SKELETALANIMATION_BUFFER_H
