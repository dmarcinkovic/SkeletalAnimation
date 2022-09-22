#ifndef SKELETALANIMATION_FRAMEBUFFERS_H
#define SKELETALANIMATION_FRAMEBUFFERS_H

#include <vector>

#include "Framebuffer.h"
#include "ImageViews.h"

namespace Animation
{
	class Framebuffers final
	{
	private:
		std::vector<Framebuffer> m_Framebuffers;

	public:
		Framebuffers(VkDevice device, VkRenderPass renderPass, VkExtent2D extent, const ImageViews &images);

		Framebuffers(const Framebuffers &) = delete;

		Framebuffers &operator=(const Framebuffers &) = delete;

		void recreate(VkDevice device, VkRenderPass renderPass, VkExtent2D extent, const ImageViews &images);

		[[nodiscard]] std::size_t size() const;

		const Framebuffer &operator[](std::size_t index) const;

	private:
		void create(VkDevice device, VkRenderPass renderPass, VkExtent2D extent, const ImageViews &images);
	};
}

#endif //SKELETALANIMATION_FRAMEBUFFERS_H
