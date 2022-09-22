#ifndef SKELETALANIMATION_SWAPCHAIN_H
#define SKELETALANIMATION_SWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

namespace Animation
{
	class SwapChainSupportDetails final
	{
	private:
		VkSurfaceCapabilitiesKHR m_Capabilities{};

		std::vector<VkSurfaceFormatKHR> m_Formats;
		std::vector<VkPresentModeKHR> m_PresentModes;

	public:
		static SwapChainSupportDetails getSwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

		static bool isSwapChainSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);

		[[nodiscard]] VkSurfaceCapabilitiesKHR getCapabilities() const;

		[[nodiscard]] VkSurfaceFormatKHR chooseSwapSurfaceFormat() const;

		[[nodiscard]] VkPresentModeKHR chooseSwapPresentMode() const;

		[[nodiscard]] VkExtent2D chooseSwapExtent() const;

		[[nodiscard]] std::uint32_t getNumberOfImages() const;

	private:
		static std::uint32_t getNumberOfFormats(VkPhysicalDevice device, VkSurfaceKHR surface);

		static std::uint32_t getNumberOfPresetModes(VkPhysicalDevice device, VkSurfaceKHR surface);
	};

	class SwapChain final
	{
	private:
		VkSwapchainKHR m_SwapChain{};
		std::vector<VkImage> m_Images;

		VkFormat m_ImageFormat{};
		VkExtent2D m_Extent{};

		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_LogicalDevice;
		VkSurfaceKHR m_Surface;

	public:
		SwapChain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkSurfaceKHR surface);

		~SwapChain();

		SwapChain(const SwapChain &) = delete;

		void recreate();

		[[nodiscard]] VkFormat getFormat() const;

		[[nodiscard]] VkExtent2D getExtent() const;

		[[nodiscard]] const std::vector<VkImage> &getImages() const;

		[[nodiscard]] VkSwapchainKHR getSwapChain() const;

	private:
		static std::uint32_t getNumberOfSwapChainImages(VkDevice logicalDevice, VkSwapchainKHR swapChain);

		void createSwapChain(const VkSwapchainCreateInfoKHR &swapChainInfo);

		void getSwapChainImages();

		void create(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

		void destroy();

		static VkSwapchainCreateInfoKHR getBasicSwapChainInfo();
	};
}

#endif //SKELETALANIMATION_SWAPCHAIN_H
