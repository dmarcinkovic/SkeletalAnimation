#include <cassert>

#include "ImageViews.h"

namespace Animation
{
	ImageViews::ImageViews(VkDevice logicalDevice, const SwapChain &swapChain)
	{
		assert(logicalDevice != VK_NULL_HANDLE);
		create(logicalDevice, swapChain);
	}

	std::size_t ImageViews::size() const
	{
		return m_ImageViews.size();
	}

	void ImageViews::recreate(VkDevice logicalDevice, const SwapChain &swapChain)
	{
		m_ImageViews.clear();
		create(logicalDevice, swapChain);
	}

	const ImageView &ImageViews::getImageView(std::size_t index) const
	{
		assert(index < m_ImageViews.size());
		return m_ImageViews[index];
	}

	const ImageView &ImageViews::operator[](std::size_t index) const
	{
		return getImageView(index);
	}

	void ImageViews::create(VkDevice logicalDevice, const SwapChain &swapChain)
	{
		m_ImageViews.reserve(swapChain.getImages().size());
		for (auto image: swapChain.getImages())
		{
			m_ImageViews.emplace_back(logicalDevice, swapChain.getFormat(), image);
		}
	}
}