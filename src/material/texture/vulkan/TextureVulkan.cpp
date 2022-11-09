#include <cassert>
#include <cstring>
#include <spdlog/spdlog.h>

#include "TextureVulkan.h"

namespace
{
	VkImageMemoryBarrier getImageMemoryBarrier(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		return barrier;
	}

	VkBufferImageCopy getImageBufferCopy()
	{
		VkBufferImageCopy imageCopy{};
		imageCopy.bufferOffset = 0;
		imageCopy.bufferRowLength = 0;
		imageCopy.bufferImageHeight = 0;
		imageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopy.imageSubresource.mipLevel = 0;
		imageCopy.imageSubresource.baseArrayLayer = 0;
		imageCopy.imageSubresource.layerCount = 1;

		return imageCopy;
	}
}

namespace Animation
{
	TextureVulkan::TextureVulkan(const std::filesystem::path &path)
			: Texture(path), m_Device(LogicalDevice::getInstance().getDevice())
	{
	}

	void TextureVulkan::createTexture(const std::unique_ptr<UniformVulkan> &uniform)
	{
		VkDeviceSize imageSize = m_Width * m_Height * 4;
		VulkanBuffer stagingBuffer = getStagingBuffer(imageSize);

		void *data;
		vkMapMemory(m_Device, stagingBuffer.getBufferMemory(), 0, imageSize, 0, &data);
		std::memcpy(data, m_Pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(m_Device, stagingBuffer.getBufferMemory());

		createImage();
		allocateImageMemory();
		prepareImage(stagingBuffer.getBuffer());

		createTextureSampler(uniform);
	}

	TextureVulkan::~TextureVulkan()
	{
		vkDestroyImage(m_Device, m_TextureImage, nullptr);
		vkFreeMemory(m_Device, m_TextureImageMemory, nullptr);
	}

	VulkanBuffer TextureVulkan::getStagingBuffer(VkDeviceSize size)
	{
		VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		return {size, usage, properties};
	}

	VkImageCreateInfo TextureVulkan::getImageInfo() const
	{
		VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;
		VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
		VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = static_cast<std::uint32_t>(m_Width);
		imageInfo.extent.height = static_cast<std::uint32_t>(m_Height);
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

	void TextureVulkan::createImage()
	{
		VkImageCreateInfo imageInfo = getImageInfo();

		if (vkCreateImage(m_Device, &imageInfo, nullptr, &m_TextureImage) != VK_SUCCESS)
		{
			spdlog::error("Failed to create image.");
			std::exit(EXIT_FAILURE);
		}

		assert(m_TextureImage != VK_NULL_HANDLE);
	}

	void TextureVulkan::allocateImageMemory()
	{
		VkMemoryRequirements memoryRequirements;
		vkGetImageMemoryRequirements(m_Device, m_TextureImage, &memoryRequirements);

		VkMemoryAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = memoryRequirements.size;

		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		allocateInfo.memoryTypeIndex = VulkanBuffer::findMemoryType(memoryRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(m_Device, &allocateInfo, nullptr, &m_TextureImageMemory) != VK_SUCCESS)
		{
			spdlog::error("Failed to allocate image memory.");
			std::exit(EXIT_FAILURE);
		}
		assert(m_TextureImageMemory != VK_NULL_HANDLE);
		vkBindImageMemory(m_Device, m_TextureImage, m_TextureImageMemory, 0);
	}

	void TextureVulkan::transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		const LogicalDevice &device = LogicalDevice::getInstance();
		VkCommandPool commandPool = VulkanBuffer::getCommandPool();

		VkCommandBuffer commandBuffer = VulkanBuffer::beginCommand(device, commandPool);
		VkImageMemoryBarrier barrier = getImageMemoryBarrier(m_TextureImage, oldLayout, newLayout);

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		} else
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}

		vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		VulkanBuffer::endCommand(device, commandPool, commandBuffer);
	}

	void TextureVulkan::copyBufferToImage(VkBuffer buffer)
	{
		const LogicalDevice &device = LogicalDevice::getInstance();

		VkCommandPool commandPool = VulkanBuffer::getCommandPool();
		VkCommandBuffer commandBuffer = VulkanBuffer::beginCommand(device, commandPool);

		VkBufferImageCopy imageCopy = getImageBufferCopy();
		imageCopy.imageOffset = {0, 0, 0};
		imageCopy.imageExtent = {static_cast<std::uint32_t>(m_Width), static_cast<std::uint32_t>(m_Height), 1};

		VkImageLayout imageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		vkCmdCopyBufferToImage(commandBuffer, buffer, m_TextureImage, imageLayout, 1, &imageCopy);

		VulkanBuffer::endCommand(device, commandPool, commandBuffer);
	}

	void TextureVulkan::createTextureSampler(const std::unique_ptr<UniformVulkan> &uniform)
	{
		uniform->createTextureImageView(m_TextureImage);
		uniform->createTextureSampler();
		uniform->updateDescriptorSets();
	}

	void TextureVulkan::prepareImage(VkBuffer buffer)
	{
		transitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		copyBufferToImage(buffer);
		transitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}
}