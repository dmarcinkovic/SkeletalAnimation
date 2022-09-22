#ifndef SKELETALANIMATION_VULKANINSTANCE_H
#define SKELETALANIMATION_VULKANINSTANCE_H

#include <vulkan/vulkan.h>
#include <vector>

namespace Animation
{
	class VulkanInstance final
	{
	private:
		VkInstance m_Instance = VK_NULL_HANDLE;

	public:
		explicit VulkanInstance(const char *applicationName);

		~VulkanInstance();

		VulkanInstance(const VulkanInstance &) = delete;

		[[nodiscard]] VkInstance getInstance() const;

	private:
		static VkApplicationInfo getVulkanApplicationInfo(const char *applicationName);

		static std::vector<const char *> getRequiredExtensions();
	};
}

#endif //SKELETALANIMATION_VULKANINSTANCE_H
