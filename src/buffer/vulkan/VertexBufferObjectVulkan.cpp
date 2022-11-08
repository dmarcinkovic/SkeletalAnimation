#include <spdlog/spdlog.h>

#include <memory>

#include "VertexBufferObjectVulkan.h"

namespace Animation
{
	void VertexBufferObjectVulkan::bind() const
	{
		assert(m_VertexBuffer);

		std::vector<VkBuffer> vertexBuffers{m_VertexBuffer->getBuffer()};
		std::vector<VkDeviceSize> offsets{0};

		assert(vertexBuffers.size() == 1);
		assert(offsets.size() == 1);

		VkCommandBuffer commandBuffer = VulkanBuffer::getCurrentCommandBuffer();
		vkCmdBindVertexBuffers(commandBuffer, m_Binding, 1, vertexBuffers.data(), offsets.data());
	}

	void VertexBufferObjectVulkan::unbind() const
	{

	}

	void VertexBufferObjectVulkan::storeFloatData(std::uint32_t binding, const std::vector<float> &data, int dataSize)
	{
		m_DataSize = dataSize;
		m_Binding = binding;

		VkDevice device = LogicalDevice::getInstance().getDevice();
		VkDeviceSize bufferSize = sizeof(float) * data.size();

		createVertexBuffer(bufferSize);
		VulkanBuffer stagingBuffer = getStagingBuffer(bufferSize);

		{
			void *gpuData;
			vkMapMemory(device, stagingBuffer.getBufferMemory(), 0, bufferSize, 0, &gpuData);
			std::memcpy(gpuData, data.data(), bufferSize);
			vkUnmapMemory(device, stagingBuffer.getBufferMemory());
		}

		VulkanBuffer::copyBuffer(stagingBuffer.getBuffer(), m_VertexBuffer->getBuffer(), bufferSize);
	}

	VulkanBuffer VertexBufferObjectVulkan::getStagingBuffer(VkDeviceSize bufferSize)
	{
		VkBufferUsageFlags usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		return {bufferSize, usage, properties};
	}

	void VertexBufferObjectVulkan::createVertexBuffer(VkDeviceSize bufferSize)
	{
		VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		m_VertexBuffer = std::make_unique<VulkanBuffer>(bufferSize, usage, properties);
	}
}