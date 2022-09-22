#ifndef SKELETALANIMATION_IMAGEVIEWS_H
#define SKELETALANIMATION_IMAGEVIEWS_H

#include <vector>
#include <vulkan/vulkan.h>

#include "ImageView.h"
#include "SwapChain.h"

namespace Animation
{
	class ImageViews final
	{
	private:
		std::vector<ImageView> m_ImageViews;

	public:
		ImageViews(VkDevice logicalDevice, const SwapChain &swapChain);

		ImageViews(const ImageViews &) = delete;

		ImageViews &operator=(const ImageViews &) = delete;

		void recreate(VkDevice logicalDevice, const SwapChain &swapChain);

		[[nodiscard]] std::size_t size() const;

		const ImageView &operator[](std::size_t index) const;

		[[nodiscard]] const ImageView &getImageView(std::size_t index) const;

	private:
		void create(VkDevice logicalDevice, const SwapChain &swapChain);
	};
}

#endif //SKELETALANIMATION_IMAGEVIEWS_H
