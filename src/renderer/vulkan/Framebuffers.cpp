#include <cassert>

#include "Framebuffers.h"

namespace Animation
{
	Framebuffers::Framebuffers(VkDevice device, VkRenderPass renderPass, VkExtent2D extent,
							   const ImageViews &images, VkImageView depthView)
	{
		assert(device != VK_NULL_HANDLE);
		create(device, renderPass, extent, images, depthView);
	}

	std::size_t Framebuffers::size() const
	{
		return m_Framebuffers.size();
	}

	const Framebuffer &Framebuffers::operator[](std::size_t index) const
	{
		assert(index < size());
		return m_Framebuffers[index];
	}

	void Framebuffers::recreate(VkDevice device, VkRenderPass renderPass, VkExtent2D extent,
								const ImageViews &images, VkImageView depthView)
	{
		m_Framebuffers.clear();
		create(device, renderPass, extent, images, depthView);
	}

	void Framebuffers::create(VkDevice device, VkRenderPass renderPass, VkExtent2D extent,
							  const ImageViews &images, VkImageView depthView)
	{
		m_Framebuffers.reserve(images.size());
		for (int i = 0; i < images.size(); ++i)
		{
			m_Framebuffers.emplace_back(device, images[i].getView(), renderPass, extent, depthView);
		}
	}
}