#ifndef SKELETALANIMATION_COMMANDPOOL_H
#define SKELETALANIMATION_COMMANDPOOL_H

#include <vulkan/vulkan.h>

namespace Animation
{
	class CommandPool final
	{
	private:
		static constexpr int NUMBER_OF_COMMAND_BUFFERS = 2;

		VkDevice m_LogicalDevice;
		VkCommandPool m_CommandPool{};

		std::vector<VkCommandBuffer> m_CommandBuffers;

		std::uint32_t m_CurrentFrame{};

	public:
		CommandPool(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

		~CommandPool();

		CommandPool(const CommandPool &) = delete;

		void update();

		static int size();

		[[nodiscard]] VkCommandBuffer getCurrentCommandBuffer() const;

		[[nodiscard]] VkCommandPool getCommandPool() const;

	private:
		void createCommandPool(const VkCommandPoolCreateInfo &poolInfo);

		void createCommandBuffers();
	};
}

#endif //SKELETALANIMATION_COMMANDPOOL_H
