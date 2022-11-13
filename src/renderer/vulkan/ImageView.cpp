#include <spdlog/spdlog.h>
#include <cassert>

#include "ImageView.h"

namespace Animation
{
	ImageView::ImageView(VkDevice logicalDevice, VkFormat format, VkImage image)
			: m_Device(logicalDevice), m_Format(format), m_Image(image)
	{
		assert(m_Device != VK_NULL_HANDLE);

		VkImageViewCreateInfo imageViewInfo = getImageViewInfo(format, image, VK_IMAGE_ASPECT_COLOR_BIT);
		createImageView(imageViewInfo);
	}

	ImageView::~ImageView()
	{
		assert(m_Device != VK_NULL_HANDLE);
		assert(m_ImageView != VK_NULL_HANDLE);
		vkDestroyImageView(m_Device, m_ImageView, nullptr);
	}

	ImageView::ImageView(const ImageView &imageView)
			: m_Device(imageView.m_Device), m_Format(imageView.m_Format), m_Image(imageView.m_Image)
	{
		assert(m_Device != VK_NULL_HANDLE);

		VkImageViewCreateInfo imageViewInfo = getImageViewInfo(m_Format, m_Image, VK_IMAGE_ASPECT_COLOR_BIT);
		createImageView(imageViewInfo);
	}

	void ImageView::createImageView(const VkImageViewCreateInfo &imageViewInfo)
	{
		if (vkCreateImageView(m_Device, &imageViewInfo, nullptr, &m_ImageView) != VK_SUCCESS)
		{
			spdlog::error("Failed to create image view.");
			std::exit(EXIT_FAILURE);
		}

		assert(m_ImageView != VK_NULL_HANDLE);
	}

	VkImageViewCreateInfo ImageView::getImageViewInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags)
	{
		VkImageViewCreateInfo imageViewInfo{};

		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image = image;
		imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format = format;
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.subresourceRange.aspectMask = aspectFlags;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;

		return imageViewInfo;
	}

	VkImageView ImageView::getView() const
	{
		assert(m_ImageView != VK_NULL_HANDLE);
		return m_ImageView;
	}
}