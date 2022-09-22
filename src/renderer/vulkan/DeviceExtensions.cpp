#include <string>
#include <unordered_set>

#include "DeviceExtensions.h"

namespace Animation
{
	const std::vector<const char *> &DeviceExtensions::getDeviceExtensions()
	{
		static std::vector<const char *> extensions{
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		return extensions;
	}

	bool DeviceExtensions::isSupported(VkPhysicalDevice device)
	{
		std::vector<VkExtensionProperties> availableExtensions = getAvailableExtensions(device);
		const std::vector<const char*> &deviceExtensions = getDeviceExtensions();

		std::unordered_set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
		for (const auto &extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	std::vector<VkExtensionProperties> DeviceExtensions::getAvailableExtensions(VkPhysicalDevice device)
	{
		std::uint32_t numberOfExtensions = getNumberOfExtensions(device);

		std::vector<VkExtensionProperties> availableExtensions(numberOfExtensions);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &numberOfExtensions, availableExtensions.data());

		return availableExtensions;
	}

	std::uint32_t DeviceExtensions::getNumberOfExtensions(VkPhysicalDevice device)
	{
		std::uint32_t numberOfExtensions;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &numberOfExtensions, nullptr);

		return numberOfExtensions;
	}
}