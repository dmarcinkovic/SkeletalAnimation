#ifndef SKELETALANIMATION_RENDERSURFACE_H
#define SKELETALANIMATION_RENDERSURFACE_H

#include <vulkan/vulkan.h>

namespace Animation
{
	class RenderSurface final
	{
	private:
		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
		VkInstance m_Instance;

	public:
		explicit RenderSurface(VkInstance instance);

		~RenderSurface();

		RenderSurface(const RenderSurface &) = delete;

		[[nodiscard]] VkSurfaceKHR getSurface() const;
	};
}

#endif //SKELETALANIMATION_RENDERSURFACE_H
