#include <spdlog/spdlog.h>
#include <cassert>

#include "RenderPass.h"
#include "DepthTexture.h"

namespace
{
	VkAttachmentDescription getColorAttachmentDescription(VkFormat swapChainFormat)
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

	VkAttachmentDescription getDepthAttachmentDescription(VkFormat format)
	{
		VkAttachmentDescription depthAttachment{};

		depthAttachment.format = format;
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		return depthAttachment;
	}

	VkAttachmentReference getColorAttachmentReference()
	{
		VkAttachmentReference colorAttachmentReference{};

		colorAttachmentReference.attachment = 0;
		colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		return colorAttachmentReference;
	}

	VkAttachmentReference getDepthAttachmentReference()
	{
		VkAttachmentReference depthAttachmentReference{};

		depthAttachmentReference.attachment = 1;
		depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		return depthAttachmentReference;
	}

	VkSubpassDescription getSubPassDescription(VkAttachmentReference &color, VkAttachmentReference &depth)
	{
		VkSubpassDescription subPassDescription{};

		subPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subPassDescription.colorAttachmentCount = 1;
		subPassDescription.pColorAttachments = &color;
		subPassDescription.pDepthStencilAttachment = &depth;

		return subPassDescription;
	}

	VkSubpassDependency getSubPassDependency()
	{
		VkSubpassDependency dependency{};

		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		return dependency;
	}
}

namespace Animation
{
	RenderPass::RenderPass(VkPhysicalDevice physicalDevice, VkDevice device, VkFormat swapChainFormat)
			: m_LogicalDevice(device)
	{
		VkFormat depthAttachmentFormat = DepthTexture::findDepthFormat(physicalDevice);

		VkAttachmentDescription colorAttachment = getColorAttachmentDescription(swapChainFormat);
		VkAttachmentDescription depthAttachment = getDepthAttachmentDescription(depthAttachmentFormat);
		VkAttachmentReference colorReference = getColorAttachmentReference();
		VkAttachmentReference depthReference = getDepthAttachmentReference();
		VkSubpassDescription subPassDescription = getSubPassDescription(colorReference, depthReference);
		VkSubpassDependency dependency = getSubPassDependency();

		std::array<VkAttachmentDescription, 2> attachments{colorAttachment, depthAttachment};

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = attachments.size();
		renderPassInfo.pAttachments = attachments.data();
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