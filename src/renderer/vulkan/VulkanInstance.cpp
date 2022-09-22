#include <cstdint>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "VulkanInstance.h"

namespace Animation
{
	VulkanInstance::VulkanInstance(const char *applicationName)
	{
		VkApplicationInfo applicationInfo = getVulkanApplicationInfo(applicationName);

		VkInstanceCreateInfo instanceInfo{};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &applicationInfo;
		instanceInfo.enabledLayerCount = 0;
		instanceInfo.pNext = nullptr;

		std::vector<const char*> extensions = getRequiredExtensions();
		instanceInfo.enabledExtensionCount = extensions.size();
		instanceInfo.ppEnabledExtensionNames = extensions.data();

		if (vkCreateInstance(&instanceInfo, nullptr, &m_Instance) != VK_SUCCESS)
		{
			spdlog::error("Failed to create Vulkan instance.");
			std::exit(EXIT_FAILURE);
		}

		assert(m_Instance != VK_NULL_HANDLE);
	}

	VulkanInstance::~VulkanInstance()
	{
		vkDestroyInstance(m_Instance, nullptr);
	}

	VkInstance VulkanInstance::getInstance() const
	{
		assert(m_Instance != VK_NULL_HANDLE);
		return m_Instance;
	}

	VkApplicationInfo VulkanInstance::getVulkanApplicationInfo(const char *applicationName)
	{
		VkApplicationInfo applicationInfo{};

		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pApplicationName = applicationName;
		applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pEngineName = "No Engine";
		applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.apiVersion = VK_API_VERSION_1_0;

		return applicationInfo;
	}

	std::vector<const char *> VulkanInstance::getRequiredExtensions()
	{
		std::uint32_t numberOfExtensions;
		const char **extensions = glfwGetRequiredInstanceExtensions(&numberOfExtensions);

		return {extensions, extensions + numberOfExtensions};
	}
}