#include <cassert>
#include <spdlog/spdlog.h>

#include "SwapChain.h"
#include "Window.h"
#include "QueueFamily.h"

namespace Animation
{
	SwapChainSupportDetails
	SwapChainSupportDetails::getSwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		SwapChainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.m_Capabilities);

		{
			std::uint32_t numberOfFormats = getNumberOfFormats(device, surface);
			if (numberOfFormats != 0)
			{
				details.m_Formats.resize(numberOfFormats);
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &numberOfFormats, details.m_Formats.data());
			}
		}
		{
			std::uint32_t numberOfPresetModes = getNumberOfPresetModes(device, surface);
			if (numberOfPresetModes != 0)
			{
				details.m_PresentModes.resize(numberOfPresetModes);

				VkPresentModeKHR *modes = details.m_PresentModes.data();
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &numberOfPresetModes, modes);
			}
		}

		return details;
	}

	bool SwapChainSupportDetails::isSwapChainSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		SwapChainSupportDetails swapChainDetails = getSwapChainSupport(device, surface);

		return !swapChainDetails.m_Formats.empty() && !swapChainDetails.m_PresentModes.empty();
	}

	std::uint32_t SwapChainSupportDetails::getNumberOfFormats(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		std::uint32_t numberOfFormats;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &numberOfFormats, nullptr);

		return numberOfFormats;
	}

	std::uint32_t SwapChainSupportDetails::getNumberOfPresetModes(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		std::uint32_t numberOfPresetModes;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &numberOfPresetModes, nullptr);

		return numberOfPresetModes;
	}

	VkSurfaceCapabilitiesKHR SwapChainSupportDetails::getCapabilities() const
	{
		return m_Capabilities;
	}

	VkSurfaceFormatKHR SwapChainSupportDetails::chooseSwapSurfaceFormat() const
	{
		for (const auto &availableFormat: m_Formats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
				availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		assert(!m_Formats.empty());
		return m_Formats.front();
	}

	VkPresentModeKHR SwapChainSupportDetails::chooseSwapPresentMode() const
	{
		for (const auto &availablePresentMode: m_PresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D SwapChainSupportDetails::chooseSwapExtent() const
	{
		if (m_Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return m_Capabilities.currentExtent;
		}

		int width, height;
		Window::getWindow().getFramebufferSize(width, height);

		VkExtent2D extent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

		{
			std::uint32_t minImageWidth = m_Capabilities.minImageExtent.width;
			std::uint32_t maxImageWidth = m_Capabilities.maxImageExtent.width;
			extent.width = std::clamp(extent.width, minImageWidth, maxImageWidth);
		}
		{
			std::uint32_t minImageHeight = m_Capabilities.minImageExtent.height;
			std::uint32_t maxImageHeight = m_Capabilities.maxImageExtent.height;
			extent.height = std::clamp(extent.height, minImageHeight, maxImageHeight);
		}

		return extent;
	}

	std::uint32_t SwapChainSupportDetails::getNumberOfImages() const
	{
		std::uint32_t numberOfImages = m_Capabilities.minImageCount + 1;
		if (m_Capabilities.maxImageCount > 0 && numberOfImages > m_Capabilities.maxImageCount)
		{
			numberOfImages = m_Capabilities.maxImageCount;
		}

		assert(numberOfImages > 0);
		return numberOfImages;
	}

	SwapChain::SwapChain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkSurfaceKHR surface)
			: m_PhysicalDevice(physicalDevice), m_LogicalDevice(logicalDevice), m_Surface(surface)
	{
		assert(m_PhysicalDevice != VK_NULL_HANDLE);
		assert(m_LogicalDevice != VK_NULL_HANDLE);
		assert(m_Surface != VK_NULL_HANDLE);

		create(physicalDevice, surface);
	}

	SwapChain::~SwapChain()
	{
		destroy();
	}

	void SwapChain::destroy()
	{
		assert(m_LogicalDevice != VK_NULL_HANDLE);
		vkDestroySwapchainKHR(m_LogicalDevice, m_SwapChain, nullptr);
	}

	void SwapChain::recreate()
	{
		destroy();
		create(m_PhysicalDevice, m_Surface);
	}

	std::uint32_t SwapChain::getNumberOfSwapChainImages(VkDevice logicalDevice, VkSwapchainKHR swapChain)
	{
		std::uint32_t numberOfSwapChainImages;
		vkGetSwapchainImagesKHR(logicalDevice, swapChain, &numberOfSwapChainImages, nullptr);

		return numberOfSwapChainImages;
	}

	void SwapChain::createSwapChain(const VkSwapchainCreateInfoKHR &swapChainInfo)
	{
		if (vkCreateSwapchainKHR(m_LogicalDevice, &swapChainInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
		{
			spdlog::error("Failed to create swap chain.");
			std::exit(EXIT_FAILURE);
		}

		assert(m_SwapChain != VK_NULL_HANDLE);
	}

	void SwapChain::getSwapChainImages()
	{
		std::uint32_t numberOfSwapChainImages = getNumberOfSwapChainImages(m_LogicalDevice, m_SwapChain);

		m_Images.resize(numberOfSwapChainImages);
		vkGetSwapchainImagesKHR(m_LogicalDevice, m_SwapChain, &numberOfSwapChainImages, m_Images.data());
	}

	void SwapChain::create(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
	{
		SwapChainSupportDetails details = SwapChainSupportDetails::getSwapChainSupport(physicalDevice, surface);

		VkSurfaceFormatKHR surfaceFormat = details.chooseSwapSurfaceFormat();
		VkPresentModeKHR presentMode = details.chooseSwapPresentMode();
		m_ImageFormat = surfaceFormat.format;
		m_Extent = details.chooseSwapExtent();

		VkSwapchainCreateInfoKHR swapChainInfo = getBasicSwapChainInfo();
		swapChainInfo.surface = surface;

		swapChainInfo.minImageCount = details.getNumberOfImages();
		swapChainInfo.imageFormat = surfaceFormat.format;
		swapChainInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapChainInfo.imageExtent = m_Extent;

		QueueFamily family = QueueFamily::findQueueFamily(physicalDevice, surface);
		assert(family.isComplete());

		if (family.getGraphicsFamily() != family.getPresentFamily())
		{
			swapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapChainInfo.queueFamilyIndexCount = 2;
			std::uint32_t queueFamilyIndices[]{family.getGraphicsFamily(), family.getPresentFamily()};
			swapChainInfo.pQueueFamilyIndices = queueFamilyIndices;
		} else
		{
			swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapChainInfo.queueFamilyIndexCount = 0;
		}

		swapChainInfo.preTransform = details.getCapabilities().currentTransform;
		swapChainInfo.presentMode = presentMode;

		createSwapChain(swapChainInfo);
		getSwapChainImages();
	}

	VkFormat SwapChain::getFormat() const
	{
		return m_ImageFormat;
	}

	const std::vector<VkImage> &SwapChain::getImages() const
	{
		return m_Images;
	}

	VkExtent2D SwapChain::getExtent() const
	{
		return m_Extent;
	}

	VkSwapchainKHR SwapChain::getSwapChain() const
	{
		assert(m_SwapChain != VK_NULL_HANDLE);
		return m_SwapChain;
	}

	VkSwapchainCreateInfoKHR SwapChain::getBasicSwapChainInfo()
	{
		VkSwapchainCreateInfoKHR swapChainInfo{};

		swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainInfo.imageArrayLayers = 1;
		swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapChainInfo.clipped = VK_TRUE;
		swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		return swapChainInfo;
	}
}
