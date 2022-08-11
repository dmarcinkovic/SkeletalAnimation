#ifndef SKELETALANIMATION_RENDERER_H
#define SKELETALANIMATION_RENDERER_H

#include <memory>
#include <glm/vec4.hpp>

namespace Animation
{
	class Renderer
	{
	protected:
		const glm::vec4 m_ClearColor{1.0f, 1.0f, 1.0f, 1.0f};

	public:
		static std::unique_ptr<Renderer> &getRenderer();

		virtual ~Renderer() = default;

		virtual void setViewport(int width, int height) = 0;

		virtual void clearWindow() = 0;
	};
}

#endif //SKELETALANIMATION_RENDERER_H
