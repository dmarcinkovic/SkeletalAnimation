#include <spdlog/spdlog.h>
#include <cassert>

#include "CommandPool.h"
#include "QueueFamily.h"

namespace Animation
{
	CommandPool::CommandPool(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
			: m_LogicalDevice(logicalDevice)
	{
		QueueFamily family = QueueFamily::findQueueFamily(physicalDevice, surface);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = family.getGraphicsFamily();

		createCommandPool(poolInfo);
		createCommandBuffers();
	}

	CommandPool::~CommandPool()
	{
		assert(m_LogicalDevice != VK_NULL_HANDLE);
		vkDestroyCommandPool(m_LogicalDevice, m_CommandPool, nullptr);
	}

	void CommandPool::createCommandPool(const VkCommandPoolCreateInfo &poolInfo)
	{
		if (vkCreateCommandPool(m_LogicalDevice, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
		{
			spdlog::error("Failed to create command pool");
			std::exit(EXIT_FAILURE);
		}

		assert(m_CommandPool != VK_NULL_HANDLE);
	}

	void CommandPool::createCommandBuffers()
	{
		m_CommandBuffers.resize(NUMBER_OF_COMMAND_BUFFERS);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = m_CommandBuffers.size();

		if (vkAllocateCommandBuffers(m_LogicalDevice, &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
		{
			spdlog::error("Failed to allocate command buffers.");
			std::exit(EXIT_FAILURE);
		}
	}

	VkCommandBuffer CommandPool::getCurrentCommandBuffer() const
	{
		assert(m_CurrentFrame < NUMBER_OF_COMMAND_BUFFERS);
		assert(m_CommandBuffers[m_CurrentFrame] != VK_NULL_HANDLE);

		return m_CommandBuffers[m_CurrentFrame];
	}

	void CommandPool::update()
	{
		++m_CurrentFrame;
		m_CurrentFrame %= NUMBER_OF_COMMAND_BUFFERS;
	}

	int CommandPool::size()
	{
		return NUMBER_OF_COMMAND_BUFFERS;
	}

	VkCommandPool CommandPool::getCommandPool() const
	{
		return m_CommandPool;
	}
}