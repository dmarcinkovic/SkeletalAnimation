#ifndef SKELETALANIMATION_DEPTHTEXTURE_H
#define SKELETALANIMATION_DEPTHTEXTURE_H

#include <vulkan/vulkan.h>
#include <vector>

namespace Animation
{
	class DepthTexture final
	{
	private:
		VkImage m_DepthImage{};
		VkDeviceMemory m_DepthImageMemory{};
		VkImageView m_DepthImageView{};

	public:
		explicit DepthTexture(VkExtent2D extent);

		DepthTexture(const DepthTexture &) = delete;

		~DepthTexture();

		void recreate(VkExtent2D extent);

		static VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);

		[[nodiscard]] VkImageView getImageView() const;

	private:
		void create(VkExtent2D extent);

		void destroy();

		static VkFormatProperties getDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format);

		void createDepthImageView(VkFormat depthFormat, VkDevice device);
	};

}

#endif //SKELETALANIMATION_DEPTHTEXTURE_H
