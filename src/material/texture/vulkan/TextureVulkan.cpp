#include <cstring>
#include <spdlog/spdlog.h>

#include "TextureVulkan.h"
#include "VulkanImage.h"

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

	TextureVulkan::TextureVulkan(const std::uint8_t *data, int len)
			: Texture(data, len), m_Device(LogicalDevice::getInstance().getDevice())
	{
	}

	TextureVulkan::TextureVulkan(int width, int height, const std::uint8_t *pixels)
			: Texture(width, height, pixels), m_Device(LogicalDevice::getInstance().getDevice())
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
		m_TextureImageMemory = VulkanImage::allocateImageMemory(m_TextureImage, m_Device);
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

	void TextureVulkan::createImage()
	{
		VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;
		VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

		auto width = static_cast<std::uint32_t>(m_Width);
		auto height = static_cast<std::uint32_t>(m_Height);

		m_TextureImage = VulkanImage::createImage(width, height, format, usage, m_Device);
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