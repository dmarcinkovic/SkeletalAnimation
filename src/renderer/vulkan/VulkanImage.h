#ifndef SKELETALANIMATION_VULKANIMAGE_H
#define SKELETALANIMATION_VULKANIMAGE_H

#include <vulkan/vulkan.h>
#include <cstdint>

namespace Animation
{
	class VulkanImage final
	{
	public:
		VulkanImage() = delete;

		static VkImage createImage(std::uint32_t width, std::uint32_t height, VkFormat format,
								   VkImageUsageFlags usage, VkDevice device);

		static VkDeviceMemory allocateImageMemory(VkImage image, VkDevice device);

	private:
		static VkImageCreateInfo
		getImageInfo(std::uint32_t width, std::uint32_t height, VkFormat format, VkImageUsageFlags usage);
	};
}

#endif //SKELETALANIMATION_VULKANIMAGE_H
