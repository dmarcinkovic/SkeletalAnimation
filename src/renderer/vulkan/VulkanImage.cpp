#include <spdlog/spdlog.h>

#include "VulkanImage.h"
#include "VulkanBuffer.h"

namespace Animation
{
	VkImageCreateInfo
	VulkanImage::getImageInfo(std::uint32_t width, std::uint32_t height, VkFormat format, VkImageUsageFlags usage)
	{
		const VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		return imageInfo;
	}

	VkImage VulkanImage::createImage(std::uint32_t width, std::uint32_t height, VkFormat format,
							 VkImageUsageFlags usage, VkDevice device)
	{
		VkImageCreateInfo imageInfo = getImageInfo(width, height, format, usage);

		VkImage image;
		if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
		{
			spdlog::error("Failed to create image.");
			std::exit(EXIT_FAILURE);
		}

		assert(image != VK_NULL_HANDLE);
		return image;
	}

	VkDeviceMemory VulkanImage::allocateImageMemory(VkImage image, VkDevice device)
	{
		VkMemoryRequirements memoryRequirements;
		vkGetImageMemoryRequirements(device, image, &memoryRequirements);

		VkMemoryAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = memoryRequirements.size;

		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		allocateInfo.memoryTypeIndex = VulkanBuffer::findMemoryType(memoryRequirements.memoryTypeBits, properties);

		VkDeviceMemory imageMemory;
		if (vkAllocateMemory(device, &allocateInfo, nullptr, &imageMemory) != VK_SUCCESS)
		{
			spdlog::error("Failed to allocate image memory.");
			std::exit(EXIT_FAILURE);
		}
		assert(imageMemory != VK_NULL_HANDLE);
		vkBindImageMemory(device, image, imageMemory, 0);

		return imageMemory;
	}
}