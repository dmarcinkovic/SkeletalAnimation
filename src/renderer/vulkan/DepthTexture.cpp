#include <spdlog/spdlog.h>
#include <cassert>

#include "DepthTexture.h"
#include "LogicalDevice.h"
#include "VulkanImage.h"
#include "ImageView.h"

namespace Animation
{
	DepthTexture::DepthTexture(VkExtent2D extent)
	{
		assert(extent.width > 0 && extent.height > 0);
		create(extent);

		assert(m_DepthImageView != VK_NULL_HANDLE);
		assert(m_DepthImage != VK_NULL_HANDLE);
		assert(m_DepthImageMemory != VK_NULL_HANDLE);
	}

	DepthTexture::~DepthTexture()
	{
		destroy();
	}

	void DepthTexture::recreate(VkExtent2D extent)
	{
		assert(extent.width > 0 && extent.height > 0);
		destroy();
		create(extent);
	}

	void DepthTexture::create(VkExtent2D extent)
	{
		const LogicalDevice &device = LogicalDevice::getInstance();

		VkFormat format = findDepthFormat(device.getPhysicalDevice());
		VkImageUsageFlags usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		VkDevice logicalDevice = device.getDevice();

		m_DepthImage = VulkanImage::createImage(extent.width, extent.height, format, usage, logicalDevice);
		m_DepthImageMemory = VulkanImage::allocateImageMemory(m_DepthImage, logicalDevice);
		createDepthImageView(format, logicalDevice);
	}

	void DepthTexture::destroy()
	{
		VkDevice device = LogicalDevice::getInstance().getDevice();

		vkDestroyImageView(device, m_DepthImageView, nullptr);
		vkDestroyImage(device, m_DepthImage, nullptr);
		vkFreeMemory(device, m_DepthImageMemory, nullptr);
	}

	VkFormat DepthTexture::findDepthFormat(VkPhysicalDevice physicalDevice)
	{
		const std::vector<VkFormat> candidates{VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
											   VK_FORMAT_D24_UNORM_S8_UINT};
		VkFormatFeatureFlags features = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

		for (VkFormat format: candidates)
		{
			VkFormatProperties properties = getDeviceFormatProperties(physicalDevice, format);

			if ((properties.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}

		spdlog::error("Failed to find supported depth format.");
		std::exit(EXIT_FAILURE);
	}

	VkFormatProperties DepthTexture::getDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format)
	{
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &properties);

		return properties;
	}

	void DepthTexture::createDepthImageView(VkFormat depthFormat, VkDevice device)
	{
		VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
		VkImageViewCreateInfo viewInfo = ImageView::getImageViewInfo(depthFormat, m_DepthImage, aspectFlags);

		if (vkCreateImageView(device, &viewInfo, nullptr, &m_DepthImageView) != VK_SUCCESS)
		{
			spdlog::error("Failed to create depth image view.");
			std::exit(EXIT_FAILURE);
		}

		assert(m_DepthImageView != VK_NULL_HANDLE);
	}

	VkImageView DepthTexture::getImageView() const
	{
		assert(m_DepthImageView != VK_NULL_HANDLE);
		return m_DepthImageView;
	}
}