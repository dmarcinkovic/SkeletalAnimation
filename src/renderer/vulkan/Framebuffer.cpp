#include <spdlog/spdlog.h>
#include <cassert>

#include "Framebuffer.h"

namespace Animation
{
	Framebuffer::Framebuffer(VkDevice logicalDevice, VkImageView imageView, VkRenderPass renderPass,
							 VkExtent2D extent, VkImageView depthView)
			: m_LogicalDevice(logicalDevice), m_ImageView(imageView), m_RenderPass(renderPass), m_Extent(extent),
			  m_DepthView(depthView)
	{
		assert(m_LogicalDevice != VK_NULL_HANDLE);
		assert(m_ImageView != VK_NULL_HANDLE);
		assert(m_RenderPass != VK_NULL_HANDLE);
		assert(m_DepthView != VK_NULL_HANDLE);

		create();
	}

	Framebuffer::~Framebuffer()
	{
		assert(m_LogicalDevice != VK_NULL_HANDLE);
		assert(m_Framebuffer != VK_NULL_HANDLE);

		vkDestroyFramebuffer(m_LogicalDevice, m_Framebuffer, nullptr);
	}

	Framebuffer::Framebuffer(const Framebuffer &framebuffer)
			: m_LogicalDevice(framebuffer.m_LogicalDevice), m_ImageView(framebuffer.m_ImageView),
			  m_RenderPass(framebuffer.m_RenderPass), m_Extent(framebuffer.m_Extent),
			  m_DepthView(framebuffer.m_DepthView)
	{
		assert(m_LogicalDevice != VK_NULL_HANDLE);
		assert(m_ImageView != VK_NULL_HANDLE);
		assert(m_RenderPass != VK_NULL_HANDLE);
		assert(m_DepthView != VK_NULL_HANDLE);

		create();
	}

	void Framebuffer::createFramebuffer(const VkFramebufferCreateInfo &framebufferInfo)
	{
		if (vkCreateFramebuffer(m_LogicalDevice, &framebufferInfo, nullptr, &m_Framebuffer) != VK_SUCCESS)
		{
			spdlog::error("Failed to create framebuffer.");
			std::exit(EXIT_FAILURE);
		}

		assert(m_Framebuffer != VK_NULL_HANDLE);
	}

	void Framebuffer::create()
	{
		std::array<VkImageView, 2> attachments{m_ImageView, m_DepthView};

		VkFramebufferCreateInfo framebufferInfo{};

		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = m_RenderPass;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = m_Extent.width;
		framebufferInfo.height = m_Extent.height;
		framebufferInfo.layers = 1;

		createFramebuffer(framebufferInfo);
	}

	VkFramebuffer Framebuffer::getFramebuffer() const
	{
		assert(m_Framebuffer != VK_NULL_HANDLE);
		return m_Framebuffer;
	}
}