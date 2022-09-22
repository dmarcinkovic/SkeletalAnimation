#include <cstdlib>
#include <spdlog/spdlog.h>
#include <algorithm>

#include "PhysicalDevice.h"
#include "DeviceExtensions.h"
#include "SwapChain.h"
#include "QueueFamily.h"

namespace Animation
{
	PhysicalDevice::PhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
	{
		std::uint32_t numberOfDevices = getNumberOfDevices(instance);

		if (!pickSuitableDevice(instance, surface, numberOfDevices))
		{
			spdlog::error("Failed to find suitable GPU with Vulkan support.");
			std::exit(EXIT_FAILURE);
		}
	}

	std::uint32_t PhysicalDevice::getNumberOfDevices(VkInstance instance)
	{
		std::uint32_t numberOfDevices;
		vkEnumeratePhysicalDevices(instance, &numberOfDevices, nullptr);

		if (numberOfDevices == 0)
		{
			spdlog::error("Failed to find GPU with Vulkan support.");
			std::exit(EXIT_FAILURE);
		}

		return numberOfDevices;
	}

	bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		QueueFamily queueFamily = QueueFamily::findQueueFamily(device, surface);
		bool extensionsSupported = DeviceExtensions::isSupported(device);
		bool swapChainSuitable;

		if (extensionsSupported)
		{
			swapChainSuitable = SwapChainSupportDetails::isSwapChainSuitable(device, surface);
		}

		return queueFamily.isComplete() && extensionsSupported && swapChainSuitable;
	}

	bool PhysicalDevice::pickSuitableDevice(VkInstance instance, VkSurfaceKHR surface, std::uint32_t numberOfDevices)
	{
		std::vector<VkPhysicalDevice> devices(numberOfDevices);
		vkEnumeratePhysicalDevices(instance, &numberOfDevices, devices.data());

		return std::ranges::any_of(devices, [this, surface](const auto &device) {
			if (isDeviceSuitable(device, surface))
			{
				m_PhysicalDevice = device;
				return true;
			} else
			{
				return false;
			}
		});
	}

	VkPhysicalDevice PhysicalDevice::getPhysicalDevice() const
	{
		assert(m_PhysicalDevice != VK_NULL_HANDLE);
		return m_PhysicalDevice;
	}
}