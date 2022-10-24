#ifndef SKELETALANIMATION_VULKANBUFFER_H
#define SKELETALANIMATION_VULKANBUFFER_H

#include <vulkan/vulkan.h>
#include <cstdint>

#include "LogicalDevice.h"

namespace Animation
{
	class VulkanBuffer final
	{
	private:
		VkBuffer m_Buffer{};
		VkDeviceMemory m_BufferMemory{};

	public:
		VulkanBuffer() = default;

		VulkanBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

		VulkanBuffer(const VulkanBuffer &) = delete;

		~VulkanBuffer();

		VkBuffer &getBuffer();

		[[nodiscard]] VkDeviceMemory getBufferMemory() const;

		static void copyBuffer(VkBuffer sourceBuffer, VkBuffer destinationBuffer, VkDeviceSize size);

		static VkCommandBuffer getCurrentCommandBuffer();

		static std::uint32_t findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties);

		static VkCommandBuffer beginCommand(const LogicalDevice &device, VkCommandPool commandPool);

		static void endCommand(const LogicalDevice &device, VkCommandPool commandPool, VkCommandBuffer commandBuffer);

		static VkCommandPool getCommandPool();

	private:
		static void createBuffer(VkDevice device, VkBuffer &buffer, const VkBufferCreateInfo &bufferInfo);

		static VkMemoryRequirements getMemoryRequirements(VkDevice device, VkBuffer buffer);

		static void allocateBufferMemory(VkDevice device, const VkMemoryAllocateInfo &info, VkDeviceMemory &memory);

		VkMemoryAllocateInfo getAllocateInfo(VkDevice device, VkMemoryPropertyFlags properties) const;

		static void submit(const LogicalDevice &device, VkCommandBuffer commandBuffer);
	};
}

#endif //SKELETALANIMATION_VULKANBUFFER_H
