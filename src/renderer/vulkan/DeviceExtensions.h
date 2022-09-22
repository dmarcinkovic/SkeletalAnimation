#ifndef SKELETALANIMATION_DEVICEEXTENSIONS_H
#define SKELETALANIMATION_DEVICEEXTENSIONS_H

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

namespace Animation
{
	class DeviceExtensions final
	{
	public:
		DeviceExtensions() = delete;

		static const std::vector<const char*> &getDeviceExtensions();

		static bool isSupported(VkPhysicalDevice device);

	private:
		static std::vector<VkExtensionProperties> getAvailableExtensions(VkPhysicalDevice device);

		static std::uint32_t getNumberOfExtensions(VkPhysicalDevice device);
	};
}

#endif //SKELETALANIMATION_DEVICEEXTENSIONS_H
