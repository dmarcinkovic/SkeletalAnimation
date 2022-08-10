#ifndef SKELETALANIMATION_RENDERER_H
#define SKELETALANIMATION_RENDERER_H

#include <memory>

namespace Animation
{
	class Renderer
	{
	public:
		static std::unique_ptr<Renderer> &getRenderer();

		virtual ~Renderer() = default;

		virtual void setViewport(int width, int height) = 0;
	};
}

#endif //SKELETALANIMATION_RENDERER_H
