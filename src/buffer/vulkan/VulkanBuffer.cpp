#include <cassert>
#include <spdlog/spdlog.h>

#include "VulkanBuffer.h"
#include "RendererVulkan.h"

namespace
{
	VkBufferCreateInfo getBufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usage)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		return bufferInfo;
	}

	VkCommandBufferAllocateInfo getCommandBufferInfo(VkCommandPool commandPool)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		return allocInfo;
	}

	VkCommandBufferBeginInfo getCommandBufferInfo()
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		return beginInfo;
	}

	VkBufferCopy getBufferCopy(VkDeviceSize size)
	{
		VkBufferCopy bufferCopy{};
		bufferCopy.srcOffset = 0;
		bufferCopy.dstOffset = 0;
		bufferCopy.size = size;

		return bufferCopy;
	}

	Animation::RendererVulkan *getRenderer()
	{
		std::unique_ptr<Animation::Renderer> &renderer = Animation::Renderer::getRenderer();
		assert(renderer);

		auto *vulkanRenderer = dynamic_cast<Animation::RendererVulkan *>(renderer.get());
		assert(vulkanRenderer);

		return vulkanRenderer;
	}
}

namespace Animation
{
	VulkanBuffer::VulkanBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
	{
		VkDevice device = LogicalDevice::getInstance().getDevice();

		VkBufferCreateInfo bufferInfo = getBufferCreateInfo(size, usage);
		createBuffer(device, m_Buffer, bufferInfo);

		VkMemoryAllocateInfo allocateInfo = getAllocateInfo(device, properties);
		allocateBufferMemory(device, allocateInfo, m_BufferMemory);

		vkBindBufferMemory(device, m_Buffer, m_BufferMemory, 0);
	}

	VulkanBuffer::~VulkanBuffer()
	{
		VkDevice device = LogicalDevice::getInstance().getDevice();

		vkDestroyBuffer(device, m_Buffer, nullptr);
		vkFreeMemory(device, m_BufferMemory, nullptr);
	}

	VkBuffer &VulkanBuffer::getBuffer()
	{
		assert(m_Buffer != VK_NULL_HANDLE);
		return m_Buffer;
	}

	VkDeviceMemory VulkanBuffer::getBufferMemory() const
	{
		assert(m_BufferMemory != VK_NULL_HANDLE);
		return m_BufferMemory;
	}

	void VulkanBuffer::copyBuffer(VkBuffer sourceBuffer, VkBuffer destinationBuffer, VkDeviceSize size)
	{
		const LogicalDevice &device = LogicalDevice::getInstance();
		VkCommandPool commandPool = getCommandPool();

		VkCommandBufferAllocateInfo allocInfo = getCommandBufferInfo(commandPool);

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device.getDevice(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo = getCommandBufferInfo();
		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion = getBufferCopy(size);
		vkCmdCopyBuffer(commandBuffer, sourceBuffer, destinationBuffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		submit(device, commandBuffer);
		vkFreeCommandBuffers(device.getDevice(), commandPool, 1, &commandBuffer);
	}

	void VulkanBuffer::createBuffer(VkDevice device, VkBuffer &buffer, const VkBufferCreateInfo &bufferInfo)
	{
		if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
		{
			spdlog::error("Failed to create buffer.");
			std::exit(EXIT_FAILURE);
		}

		assert(buffer != VK_NULL_HANDLE);
	}

	VkMemoryRequirements VulkanBuffer::getMemoryRequirements(VkDevice device, VkBuffer buffer)
	{
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

		return memoryRequirements;
	}

	void VulkanBuffer::allocateBufferMemory(VkDevice device, const VkMemoryAllocateInfo &info, VkDeviceMemory &memory)
	{
		if (vkAllocateMemory(device, &info, nullptr, &memory) != VK_SUCCESS)
		{
			spdlog::error("Failed to allocate buffer memory.");
			std::exit(EXIT_FAILURE);
		}

		assert(memory != VK_NULL_HANDLE);
	}

	std::uint32_t VulkanBuffer::findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDevice physicalDevice = LogicalDevice::getInstance().getPhysicalDevice();
		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

		for (std::uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		spdlog::error("Failed to find suitable memory type.");
		std::exit(EXIT_FAILURE);
	}

	VkMemoryAllocateInfo VulkanBuffer::getAllocateInfo(VkDevice device, VkMemoryPropertyFlags properties) const
	{
		VkMemoryRequirements memoryRequirements = getMemoryRequirements(device, m_Buffer);

		VkMemoryAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = memoryRequirements.size;
		allocateInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, properties);

		return allocateInfo;
	}

	void VulkanBuffer::submit(const LogicalDevice &device, VkCommandBuffer commandBuffer)
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VkQueue graphicsQueue = device.getGraphicsQueue();
		VkQueue presentQueue = device.getGraphicsQueue();

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(presentQueue);
	}

	VkCommandPool VulkanBuffer::getCommandPool()
	{
		return getRenderer()->getCommandPool().getCommandPool();
	}

	VkCommandBuffer VulkanBuffer::getCurrentCommandBuffer()
	{
		return getRenderer()->getCurrentCommandBuffer();
	}
}