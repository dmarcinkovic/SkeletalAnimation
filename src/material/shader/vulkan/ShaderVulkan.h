#ifndef SKELETALANIMATION_SHADERVULKAN_H
#define SKELETALANIMATION_SHADERVULKAN_H

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

#include "Shader.h"
#include "LogicalDevice.h"
#include "RenderPass.h"

namespace Animation
{
	class ShaderVulkan final : public Shader
	{
	private:
		VkPipelineLayout m_PipelineLayout{};
		VkPipeline m_GraphicsPipeline{};
		VkDescriptorSetLayout m_DescriptorSetLayout{};

	public:
		ShaderVulkan();

		~ShaderVulkan() override;

		void startShader() const override;

		void stopShader() const override;

		void setTexture(const std::unique_ptr<Texture> &texture) override;

	private:
		void createShader(const LogicalDevice &device);

		void createPipeline(const LogicalDevice &device, const VkGraphicsPipelineCreateInfo &pipelineInfo);

		static VkShaderModule createShader(const LogicalDevice &device, const std::uint8_t *data, std::uint32_t size);

		void createPipelineLayout(const LogicalDevice &device);

		void createDescriptorLayout(const LogicalDevice &device);

		static std::vector<VkVertexInputBindingDescription> getBindingDescription();

		static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();

		static class RendererVulkan *getRenderer();

		static const RenderPass &getRenderPass();

		static void setViewport(VkCommandBuffer commandBuffer, VkExtent2D extent);

		static void setScissor(VkCommandBuffer commandBuffer, VkExtent2D extent);

		void bindUniforms(VkCommandBuffer commandBuffer) const;

		static class TextureVulkan *getTexture(const std::unique_ptr<Texture> &texture);

		[[nodiscard]] std::unique_ptr<class UniformVulkan> createUniform() const;
	};
}

#endif //SKELETALANIMATION_SHADERVULKAN_H
