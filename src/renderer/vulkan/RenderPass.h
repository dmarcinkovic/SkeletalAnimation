#ifndef SKELETALANIMATION_RENDERPASS_H
#define SKELETALANIMATION_RENDERPASS_H

#include <vulkan/vulkan.h>

namespace Animation
{
	class RenderPass final
	{
	private:
		VkRenderPass m_RenderPass{};
		VkDevice m_LogicalDevice;

	public:
		RenderPass(VkDevice device, VkFormat swapChainFormat);

		~RenderPass();

		RenderPass(const RenderPass &) = delete;

		[[nodiscard]] VkRenderPass getRenderPass() const;

	private:
		static VkAttachmentDescription getColorAttachmentDescription(VkFormat swapChainFormat);

		static VkAttachmentReference getAttachmentReference();

		static VkSubpassDescription getSubPassDescription(VkAttachmentReference &attachmentReference);

		static VkSubpassDependency getSubPassDependency();

		void createRenderPass(const VkRenderPassCreateInfo &renderPassInfo);
	};
}

#endif //SKELETALANIMATION_RENDERPASS_H
