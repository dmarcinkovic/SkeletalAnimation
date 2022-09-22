#ifndef SKELETALANIMATION_QUEUEFAMILY_H
#define SKELETALANIMATION_QUEUEFAMILY_H

#include <vulkan/vulkan.h>
#include <optional>
#include <cstdint>

namespace Animation
{
	class QueueFamily final
	{
	private:
		std::optional<std::uint32_t> m_GraphicsFamily{};
		std::optional<std::uint32_t> m_PresentFamily{};

	public:
		static QueueFamily findQueueFamily(VkPhysicalDevice device, VkSurfaceKHR surface);

		static std::vector<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice device);

		[[nodiscard]] std::uint32_t getGraphicsFamily() const;

		[[nodiscard]] std::uint32_t getPresentFamily() const;

		[[nodiscard]] bool isComplete() const;
	};
}

#endif //SKELETALANIMATION_QUEUEFAMILY_H
