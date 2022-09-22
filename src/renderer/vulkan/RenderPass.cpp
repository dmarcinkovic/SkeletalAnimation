#include <spdlog/spdlog.h>
#include <cassert>

#include "RenderPass.h"

namespace Animation
{
	RenderPass::RenderPass(VkDevice device, VkFormat swapChainFormat)
			: m_LogicalDevice(device)
	{
		VkAttachmentDescription colorAttachment = getColorAttachmentDescription(swapChainFormat);
		VkAttachmentReference colorAttachmentReference = getAttachmentReference();
		VkSubpassDescription subPassDescription = getSubPassDescription(colorAttachmentReference);
		VkSubpassDependency dependency = getSubPassDependency();

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subPassDescription;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		createRenderPass(renderPassInfo);
	}

	RenderPass::~RenderPass()
	{
		assert(m_LogicalDevice != VK_NULL_HANDLE);
		vkDestroyRenderPass(m_LogicalDevice, m_RenderPass, nullptr);
	}

	VkRenderPass RenderPass::getRenderPass() const
	{
		assert(m_RenderPass != VK_NULL_HANDLE);
		return m_RenderPass;
	}

	VkAttachmentDescription RenderPass::getColorAttachmentDescription(VkFormat swapChainFormat)
	{
		VkAttachmentDescription colorAttachment{};

		colorAttachment.format = swapChainFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		return colorAttachment;
	}

	VkAttachmentReference RenderPass::getAttachmentReference()
	{
		VkAttachmentReference colorAttachmentReference{};

		colorAttachmentReference.attachment = 0;
		colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		return colorAttachmentReference;
	}

	VkSubpassDescription RenderPass::getSubPassDescription(VkAttachmentReference &attachmentReference)
	{
		VkSubpassDescription subPassDescription{};

		subPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subPassDescription.colorAttachmentCount = 1;
		subPassDescription.pColorAttachments = &attachmentReference;

		return subPassDescription;
	}

	VkSubpassDependency RenderPass::getSubPassDependency()
	{
		VkSubpassDependency dependency{};

		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		return dependency;
	}

	void RenderPass::createRenderPass(const VkRenderPassCreateInfo &renderPassInfo)
	{
		if (vkCreateRenderPass(m_LogicalDevice, &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
		{
			spdlog::error("Failed to create render pass.");
			std::exit(EXIT_FAILURE);
		}

		assert(m_RenderPass != VK_NULL_HANDLE);
	}
}