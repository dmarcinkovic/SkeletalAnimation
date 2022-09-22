#ifndef SKELETALANIMATION_FRAMEBUFFER_H
#define SKELETALANIMATION_FRAMEBUFFER_H

#include <vulkan/vulkan.h>

namespace Animation
{
	class Framebuffer final
	{
	private:
		VkDevice m_LogicalDevice;
		VkFramebuffer m_Framebuffer{};

		VkImageView m_ImageView;
		VkRenderPass m_RenderPass;
		VkExtent2D m_Extent;

	public:
		Framebuffer(VkDevice logicalDevice, VkImageView imageView, VkRenderPass renderPass, VkExtent2D extent);

		~Framebuffer();

		Framebuffer(const Framebuffer &framebuffer);

		VkFramebuffer getFramebuffer() const;

	private:
		void createFramebuffer(const VkFramebufferCreateInfo &framebufferInfo);

		void create();
	};
}

#endif //SKELETALANIMATION_FRAMEBUFFER_H
