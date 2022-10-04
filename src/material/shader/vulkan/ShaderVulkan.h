#ifndef SKELETALANIMATION_SHADERVULKAN_H
#define SKELETALANIMATION_SHADERVULKAN_H

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

#include "Shader.h"
#include "LogicalDevice.h"
#include "RenderPass.h"
#include "RendererVulkan.h"

namespace Animation
{
	class ShaderVulkan final : public Shader
	{
	private:
		VkPipelineLayout m_PipelineLayout{};
		VkPipeline m_GraphicsPipeline{};

	public:
		ShaderVulkan();

		~ShaderVulkan() override;

		void startShader() const override;

		void stopShader() const override;

	private:
		void createShader(const LogicalDevice &device);

		void createPipeline(const LogicalDevice &device, const VkGraphicsPipelineCreateInfo &pipelineInfo);

		static VkShaderModule createShader(const LogicalDevice &device, const std::uint8_t *data, std::uint32_t size);

		void createPipelineLayout(const LogicalDevice &device);

		static VkVertexInputBindingDescription getBindingDescription();

		static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();

		static RendererVulkan *getRenderer();

		static const RenderPass &getRenderPass();

		static void beginCommandBuffer(VkCommandBuffer commandBuffer);

		static void beginRenderPass(const RendererVulkan *renderer, VkCommandBuffer commandBuffer);

		static void setViewport(VkCommandBuffer commandBuffer, VkExtent2D extent);

		static void setScissor(VkCommandBuffer commandBuffer, VkExtent2D extent);

		void setUniforms();

		void bindUniforms(VkCommandBuffer commandBuffer) const;
	};
}

#endif //SKELETALANIMATION_SHADERVULKAN_H
