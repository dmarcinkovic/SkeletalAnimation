#ifndef SKELETALANIMATION_RENDERERVULKAN_H
#define SKELETALANIMATION_RENDERERVULKAN_H

#include <vulkan/vulkan.h>

#include "Renderer.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "VulkanInstance.h"
#include "RenderSurface.h"
#include "SwapChain.h"
#include "ImageViews.h"
#include "RenderPass.h"
#include "Framebuffers.h"
#include "CommandPool.h"
#include "SyncObjects.h"
#include "DepthTexture.h"

namespace Animation
{
	class RendererVulkan final : public Renderer
	{
	private:
		LogicalDevice &m_Device;

		SwapChain m_SwapChain;
		ImageViews m_ImageViews;
		RenderPass m_RenderPass;
		CommandPool m_CommandPool;

		DepthTexture m_DepthTexture;
		Framebuffers m_Framebuffers;
		SyncObjects m_SyncObjects;

		VkClearValue m_Color{};

		std::uint32_t m_ImageIndex{};
		bool m_FramebufferResized{};

	public:
		static std::unique_ptr<Renderer> &getRenderer();

		RendererVulkan(const RendererVulkan &) = delete;

		~RendererVulkan() override;

		void preRender() override;

		void postRender() override;

		void setViewport(int width, int height) override;

		void clearWindow() override;

		void draw(std::uint64_t vertexCount) const override;

		[[nodiscard]] std::unique_ptr<VertexArrayObject> createVertexArrayObject() const override;

		[[nodiscard]] std::unique_ptr<VertexBufferObject> createVertexBufferObject() const override;

		[[nodiscard]] std::unique_ptr<Shader> getShader() const override;

		[[nodiscard]] std::unique_ptr<Texture> getTexture(const std::filesystem::path &path) const override;

		[[nodiscard]] std::unique_ptr<Texture> getTexture(const std::uint8_t *data, int len) const override;

		[[nodiscard]] std::unique_ptr<Texture> getTexture(const std::uint8_t *pixels, int w, int h) const override;

		[[nodiscard]] glm::mat4 getProjectionMatrix(const Camera &camera) const override;

		[[nodiscard]] const RenderPass &getRenderPass() const;

		[[nodiscard]] VkCommandBuffer getCurrentCommandBuffer() const;

		[[nodiscard]] const SwapChain &getSwapChain() const;

		[[nodiscard]] VkClearValue getClearColor() const;

		[[nodiscard]] const Framebuffer &getCurrentFramebuffer() const;

		[[nodiscard]] const CommandPool &getCommandPool() const;

		static void beginCommandBuffer(VkCommandBuffer commandBuffer);

		void beginRenderPass(VkCommandBuffer commandBuffer);

	private:
		RendererVulkan();

		void recreateSwapChain();

		[[nodiscard]] std::vector<VkSemaphore> getSignalSemaphores() const;

		[[nodiscard]] VkResult presentQueue(const std::vector<VkSemaphore> &signalSemaphores) const;

		void submit(const VkSubmitInfo &info) const;
	};
}

#endif //SKELETALANIMATION_RENDERERVULKAN_H
