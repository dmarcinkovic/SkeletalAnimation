#include <vector>
#include <cassert>

#include "QueueFamily.h"

namespace Animation
{
	QueueFamily QueueFamily::findQueueFamily(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		QueueFamily family;
		std::vector<VkQueueFamilyProperties> queueFamilies = getQueueFamilyProperties(device);

		for (int i = 0; i < queueFamilies.size(); ++i)
		{
			if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				family.m_GraphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

			if (presentSupport)
			{
				family.m_PresentFamily = i;
			}

			if (family.isComplete())
			{
				break;
			}
		}

		return family;
	}

	bool QueueFamily::isComplete() const
	{
		return m_GraphicsFamily.has_value() && m_PresentFamily.has_value();
	}

	std::vector<VkQueueFamilyProperties> QueueFamily::getQueueFamilyProperties(VkPhysicalDevice device)
	{
		std::uint32_t numberOfQueueFamilies = 0;

		vkGetPhysicalDeviceQueueFamilyProperties(device, &numberOfQueueFamilies, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(numberOfQueueFamilies);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &numberOfQueueFamilies, queueFamilies.data());

		return queueFamilies;
	}

	std::uint32_t QueueFamily::getGraphicsFamily() const
	{
		assert(m_GraphicsFamily.has_value());
		return m_GraphicsFamily.value();
	}

	std::uint32_t QueueFamily::getPresentFamily() const
	{
		assert(m_PresentFamily.has_value());
		return m_PresentFamily.value();
	}
}