#include <cstring>
#include <memory>

#include "IndexBufferVulkan.h"
#include "LogicalDevice.h"

namespace Animation
{
	void IndexBufferVulkan::bind() const
	{
		assert(m_IndexBuffer);
		VkCommandBuffer commandBuffer = VulkanBuffer::getCurrentCommandBuffer();
		vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
	}

	void IndexBufferVulkan::unbind() const
	{

	}

	void IndexBufferVulkan::storeData(const std::vector<std::uint32_t> &indices)
	{
		VkDevice device = LogicalDevice::getInstance().getDevice();
		VkDeviceSize bufferSize = sizeof(std::uint32_t) * indices.size();

		createIndexBuffer(bufferSize);
		VulkanBuffer stagingBuffer = getStagingBuffer(bufferSize);

		{
			void *gpuData;
			vkMapMemory(device, stagingBuffer.getBufferMemory(), 0, bufferSize, 0, &gpuData);
			std::memcpy(gpuData, indices.data(), bufferSize);
			vkUnmapMemory(device, stagingBuffer.getBufferMemory());
		}

		VulkanBuffer::copyBuffer(stagingBuffer.getBuffer(), m_IndexBuffer->getBuffer(), bufferSize);
	}

	VulkanBuffer IndexBufferVulkan::getStagingBuffer(VkDeviceSize bufferSize)
	{
		VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		return {bufferSize, usage, properties};
	}

	void IndexBufferVulkan::createIndexBuffer(VkDeviceSize bufferSize)
	{
		VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		m_IndexBuffer = std::make_unique<VulkanBuffer>(bufferSize, usage, properties);
	}
}