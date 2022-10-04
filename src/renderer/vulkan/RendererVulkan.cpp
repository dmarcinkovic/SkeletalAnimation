#include <spdlog/spdlog.h>

#include "RendererVulkan.h"
#include "VulkanInstance.h"
#include "VertexArrayObjectVulkan.h"
#include "VertexBufferObjectVulkan.h"
#include "ShaderVulkan.h"
#include "Window.h"

namespace Animation
{
	std::unique_ptr<Renderer> &RendererVulkan::getRenderer()
	{
		static std::unique_ptr<Renderer> renderer(new RendererVulkan());

		return renderer;
	}

	RendererVulkan::RendererVulkan()
			: m_Device(LogicalDevice::getInstance()),
			  m_SwapChain(m_Device.getPhysicalDevice(), m_Device.getDevice(), m_Device.getSurface()),
			  m_ImageViews(m_Device.getDevice(), m_SwapChain),
			  m_RenderPass(m_Device.getDevice(), m_SwapChain.getFormat()),
			  m_Framebuffers(m_Device.getDevice(), m_RenderPass.getRenderPass(), m_SwapChain.getExtent(), m_ImageViews),
			  m_CommandPool(m_Device.getDevice(), m_Device.getPhysicalDevice(), m_Device.getSurface()),
			  m_SyncObjects(m_Device.getDevice(), Animation::CommandPool::size())
	{
		spdlog::info("Initialized Vulkan renderer.");
	}

	RendererVulkan::~RendererVulkan()
	{
		vkDeviceWaitIdle(m_Device.getDevice());
	}

	void RendererVulkan::setViewport(int, int)
	{
		m_FramebufferResized = true;
	}

	void RendererVulkan::clearWindow()
	{
		m_Color = VkClearValue{.color{.float32{m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a}}};
	}

	void RendererVulkan::draw(std::uint64_t vertexCount) const
	{
		VkCommandBuffer commandBuffer = m_CommandPool.getCurrentCommandBuffer();
		vkCmdDrawIndexed(commandBuffer, vertexCount, 1, 0, 0, 0);
	}

	std::unique_ptr<VertexArrayObject> RendererVulkan::createVertexArrayObject() const
	{
		return std::make_unique<VertexArrayObjectVulkan>();
	}

	std::unique_ptr<VertexBufferObject> RendererVulkan::createVertexBufferObject() const
	{
		return std::make_unique<VertexBufferObjectVulkan>();
	}

	const RenderPass &RendererVulkan::getRenderPass() const
	{
		return m_RenderPass;
	}

	VkCommandBuffer RendererVulkan::getCurrentCommandBuffer() const
	{
		return m_CommandPool.getCurrentCommandBuffer();
	}

	const SwapChain &RendererVulkan::getSwapChain() const
	{
		return m_SwapChain;
	}

	VkClearValue RendererVulkan::getClearColor() const
	{
		return m_Color;
	}

	const Framebuffer &RendererVulkan::getCurrentFramebuffer() const
	{
		assert(m_ImageIndex < m_Framebuffers.size());
		return m_Framebuffers[m_ImageIndex];
	}

	std::unique_ptr<Shader> RendererVulkan::getShader() const
	{
		return std::make_unique<ShaderVulkan>();
	}

	const CommandPool &RendererVulkan::getCommandPool() const
	{
		return m_CommandPool;
	}

	void RendererVulkan::preRender()
	{
		VkFence fence = m_SyncObjects.getCurrentFence();
		vkWaitForFences(m_Device.getDevice(), 1, &fence, VK_TRUE, UINT64_MAX);

		VkSemaphore imageSemaphore = m_SyncObjects.getCurrentImageSemaphore();
		VkResult result = vkAcquireNextImageKHR(m_Device.getDevice(), m_SwapChain.getSwapChain(),
												std::numeric_limits<std::uint64_t>::max(), imageSemaphore,
												VK_NULL_HANDLE, &m_ImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return;
		} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			spdlog::error("Failed to acquire swap chain image.");
			std::exit(EXIT_FAILURE);
		}

		vkResetFences(m_Device.getDevice(), 1, &fence);

		vkResetCommandBuffer(m_CommandPool.getCurrentCommandBuffer(), 0);
	}

	void RendererVulkan::postRender()
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore currentImageSemaphore = m_SyncObjects.getCurrentImageSemaphore();
		VkSemaphore waitSemaphores[] = {currentImageSemaphore};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		VkCommandBuffer currentCommandBuffer = m_CommandPool.getCurrentCommandBuffer();
		submitInfo.pCommandBuffers = &currentCommandBuffer;

		std::vector<VkSemaphore> signalSemaphores = getSignalSemaphores();
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores.data();

		submit(submitInfo);

		VkResult result = presentQueue(signalSemaphores);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_FramebufferResized)
		{
			m_FramebufferResized = false;
			recreateSwapChain();
		} else if (result != VK_SUCCESS)
		{
			spdlog::error("Failed to present swap chain image.");
			std::exit(EXIT_FAILURE);
		}

		m_SyncObjects.update();
		m_CommandPool.update();
	}

	void RendererVulkan::recreateSwapChain()
	{
		int width = 0, height = 0;
		Window::getWindow().getValidFramebufferSize(width, height);
		assert(width > 0 && height > 0);

		vkDeviceWaitIdle(m_Device.getDevice());

		VkDevice device = m_Device.getDevice();

		m_SwapChain.recreate();
		m_ImageViews.recreate(device, m_SwapChain);
		m_Framebuffers.recreate(device, m_RenderPass.getRenderPass(), m_SwapChain.getExtent(), m_ImageViews);
	}

	VkResult RendererVulkan::presentQueue(const std::vector<VkSemaphore> &signalSemaphores) const
	{
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores.data();

		VkSwapchainKHR swapChain = m_SwapChain.getSwapChain();
		VkSwapchainKHR swapChains[] = {swapChain};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &m_ImageIndex;

		return vkQueuePresentKHR(m_Device.getPresentQueue(), &presentInfo);
	}

	std::vector<VkSemaphore> RendererVulkan::getSignalSemaphores() const
	{
		VkSemaphore currentRenderSemaphore = m_SyncObjects.getCurrentRenderSemaphore();
		std::vector<VkSemaphore> signalSemaphores{currentRenderSemaphore};

		return signalSemaphores;
	}

	void RendererVulkan::submit(const VkSubmitInfo &info) const
	{
		VkFence fence = m_SyncObjects.getCurrentFence();
		if (vkQueueSubmit(m_Device.getGraphicsQueue(), 1, &info, fence) != VK_SUCCESS)
		{
			spdlog::error("Failed to submit draw command buffer.");
			std::exit(EXIT_FAILURE);
		}
	}
}