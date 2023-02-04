#ifndef SKELETALANIMATION_LOGICALDEVICE_H
#define SKELETALANIMATION_LOGICALDEVICE_H

#include <vulkan/vulkan.h>
#include <vector>

#include "QueueFamily.h"
#include "RenderSurface.h"
#include "VulkanInstance.h"
#include "PhysicalDevice.h"

namespace Animation
{
	class LogicalDevice final
	{
	private:
		VulkanInstance m_Instance;
		RenderSurface m_RenderSurface;
		PhysicalDevice m_PhysicalDevice;

		VkDevice m_LogicalDevice = VK_NULL_HANDLE;
		VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
		VkQueue m_PresentQueue = VK_NULL_HANDLE;

	public:
		static LogicalDevice &getInstance();

		~LogicalDevice();

		LogicalDevice(const LogicalDevice &) = delete;

		[[nodiscard]] VkSurfaceKHR getSurface() const;

		[[nodiscard]] VkPhysicalDevice getPhysicalDevice() const;

		[[nodiscard]] VkDevice getDevice() const;

		[[nodiscard]] VkQueue getGraphicsQueue() const;

		[[nodiscard]] VkQueue getPresentQueue() const;

	private:
		LogicalDevice();

		static std::vector<VkDeviceQueueCreateInfo> getQueueCreateInfo(const QueueFamily &family);

		void getDeviceQueues(const QueueFamily &family);
	};
}

#endif //SKELETALANIMATION_LOGICALDEVICE_H
