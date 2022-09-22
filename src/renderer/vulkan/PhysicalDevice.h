#ifndef SKELETALANIMATION_PHYSICALDEVICE_H
#define SKELETALANIMATION_PHYSICALDEVICE_H

#include <vulkan/vulkan.h>
#include <cstdint>

namespace Animation
{
	class PhysicalDevice final
	{
	private:
		VkPhysicalDevice m_PhysicalDevice{VK_NULL_HANDLE};

	public:
		PhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

		PhysicalDevice(const PhysicalDevice &) = delete;

		[[nodiscard]] VkPhysicalDevice getPhysicalDevice() const;

	private:
		static std::uint32_t getNumberOfDevices(VkInstance instance);

		bool pickSuitableDevice(VkInstance instance, VkSurfaceKHR surface, std::uint32_t numberOfDevices);

		static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
	};
}

#endif //SKELETALANIMATION_PHYSICALDEVICE_H
