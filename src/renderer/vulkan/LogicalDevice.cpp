#include <unordered_set>
#include <cassert>
#include <spdlog/spdlog.h>

#include "LogicalDevice.h"
#include "DeviceExtensions.h"
#include "Window.h"

namespace Animation
{
	LogicalDevice::LogicalDevice()
			: m_Instance(Window::getWindowTitle()), m_RenderSurface(m_Instance.getInstance()),
			  m_PhysicalDevice(m_Instance.getInstance(), m_RenderSurface.getSurface())
	{
		VkPhysicalDevice device = m_PhysicalDevice.getPhysicalDevice();
		QueueFamily family = QueueFamily::findQueueFamily(device, m_RenderSurface.getSurface());

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfo = getQueueCreateInfo(family);
		const std::vector<const char *> deviceExtensions = DeviceExtensions::getDeviceExtensions();

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo deviceInfo{};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.queueCreateInfoCount = queueCreateInfo.size();
		deviceInfo.pQueueCreateInfos = queueCreateInfo.data();
		deviceInfo.pEnabledFeatures = &deviceFeatures;
		deviceInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
		deviceInfo.enabledLayerCount = 0;

		if (vkCreateDevice(device, &deviceInfo, nullptr, &m_LogicalDevice) != VK_SUCCESS)
		{
			spdlog::error("Failed to create logical device.");
			std::exit(EXIT_FAILURE);
		}

		assert(m_LogicalDevice != VK_NULL_HANDLE);
		getDeviceQueues(family);
	}

	LogicalDevice &LogicalDevice::getInstance()
	{
		static LogicalDevice device;
		return device;
	}

	LogicalDevice::~LogicalDevice()
	{
		vkDestroyDevice(m_LogicalDevice, nullptr);
	}

	VkInstance LogicalDevice::getVulkanInstance() const
	{
		return m_Instance.getInstance();
	}

	VkSurfaceKHR LogicalDevice::getSurface() const
	{
		return m_RenderSurface.getSurface();
	}

	VkPhysicalDevice LogicalDevice::getPhysicalDevice() const
	{
		return m_PhysicalDevice.getPhysicalDevice();
	}

	VkQueue LogicalDevice::getGraphicsQueue() const
	{
		return m_GraphicsQueue;
	}

	VkQueue LogicalDevice::getPresentQueue() const
	{
		return m_PresentQueue;
	}

	std::vector<VkDeviceQueueCreateInfo> LogicalDevice::getQueueCreateInfo(const QueueFamily &family)
	{
		std::vector<VkDeviceQueueCreateInfo> result;

		assert(family.isComplete());
		std::unordered_set<std::uint32_t> uniqueQueueFamilies = {family.getGraphicsFamily(), family.getPresentFamily()};

		float queuePriority = 1.0f;
		for (std::uint32_t queueFamily: uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			result.emplace_back(queueCreateInfo);
		}

		return result;
	}

	VkDevice LogicalDevice::getDevice() const
	{
		assert(m_LogicalDevice != VK_NULL_HANDLE);
		return m_LogicalDevice;
	}

	void LogicalDevice::getDeviceQueues(const QueueFamily &family)
	{
		assert(family.isComplete());
		vkGetDeviceQueue(m_LogicalDevice, family.getGraphicsFamily(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_LogicalDevice, family.getPresentFamily(), 0, &m_PresentQueue);

		assert(m_GraphicsQueue != VK_NULL_HANDLE);
		assert(m_PresentQueue != VK_NULL_HANDLE);
	}
}