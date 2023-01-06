#include <spdlog/spdlog.h>
#include <glm/ext/matrix_clip_space.hpp>

#include "RendererVulkan.h"
#include "VulkanInstance.h"
#include "VertexArrayObjectVulkan.h"
#include "VertexBufferObjectVulkan.h"
#include "ShaderVulkan.h"
#include "TextureVulkan.h"
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
			  m_RenderPass(m_Device.getPhysicalDevice(), m_Device.getDevice(), m_SwapChain.getFormat()),
			  m_CommandPool(m_Device.getDevice(), m_Device.getPhysicalDevice(), m_Device.getSurface()),
			  m_DepthTexture(m_SwapChain.getExtent()),
			  m_Framebuffers(m_Device.getDevice(), m_RenderPass.getRenderPass(), m_SwapChain.getExtent(), m_ImageViews,
							 m_DepthTexture.getImageView()),
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

		VkCommandBuffer commandBuffer = m_CommandPool.getCurrentCommandBuffer();
		vkResetCommandBuffer(commandBuffer, 0);

		beginCommandBuffer(commandBuffer);
		beginRenderPass(commandBuffer);
	}

	void RendererVulkan::postRender()
	{
		VkCommandBuffer commandBuffer = m_CommandPool.getCurrentCommandBuffer();
		vkCmdEndRenderPass(commandBuffer);
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			spdlog::error("Failed to record command buffer.");
			std::exit(EXIT_FAILURE);
		}

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore currentImageSemaphore = m_SyncObjects.getCurrentImageSemaphore();
		VkSemaphore waitSemaphores[] = {currentImageSemaphore};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

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
		m_DepthTexture.recreate(m_SwapChain.getExtent());

		VkRenderPass renderPass = m_RenderPass.getRenderPass();
		VkImageView depthView = m_DepthTexture.getImageView();
		m_Framebuffers.recreate(device, renderPass, m_SwapChain.getExtent(), m_ImageViews, depthView);
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

	std::unique_ptr<Texture> RendererVulkan::getTexture(const std::filesystem::path &path) const
	{
		return std::make_unique<TextureVulkan>(path);
	}

	std::unique_ptr<Texture> RendererVulkan::getTexture(const std::uint8_t *data, int len) const
	{
		return std::make_unique<TextureVulkan>(data, len);
	}

	std::unique_ptr<Texture> RendererVulkan::getTexture(const std::uint8_t *pixels, int width, int height) const
	{
		return std::make_unique<TextureVulkan>(width, height, pixels);
	}

	glm::mat4 RendererVulkan::getProjectionMatrix(const Camera &camera) const
	{
		VkExtent2D extent = m_SwapChain.getExtent();

		assert(extent.width > 0);
		assert(extent.height > 0);

		const float aspect = static_cast<float>(extent.width) / static_cast<float>(extent.height);
		const float fov = camera.getFov();
		const float near = camera.getNear();
		const float far = camera.getFar();

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
		projectionMatrix[1][1] *= -1;

		return projectionMatrix;
	}

	void RendererVulkan::beginCommandBuffer(VkCommandBuffer commandBuffer)
	{
		VkCommandBufferBeginInfo beginCommandBuffer{.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

		if (vkBeginCommandBuffer(commandBuffer, &beginCommandBuffer) != VK_SUCCESS)
		{
			spdlog::error("Failed to begin command buffer.");
			std::exit(EXIT_FAILURE);
		}
	}

	void RendererVulkan::beginRenderPass(VkCommandBuffer commandBuffer)
	{
		VkClearValue clearColor = getClearColor();
		VkClearValue depthStencil = {1.0f, 0};
		std::array<VkClearValue, 2> clearValues{clearColor, depthStencil};
		VkRenderPassBeginInfo renderPassInfo{};

		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_RenderPass.getRenderPass();
		renderPassInfo.framebuffer = getCurrentFramebuffer().getFramebuffer();
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = m_SwapChain.getExtent();
		renderPassInfo.clearValueCount = clearValues.size();
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}
}