#include <spdlog/spdlog.h>
#include <cassert>

#include "ShaderVulkan.h"
#include "StandardVertexShader.spv.h"
#include "StandardFragmentShader.spv.h"
#include "RendererVulkan.h"
#include "UniformVulkan.h"

namespace
{
	VkPipelineShaderStageCreateInfo createShaderInfo(VkShaderStageFlagBits stage, VkShaderModule shader)
	{
		VkPipelineShaderStageCreateInfo shaderInfo{};

		shaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderInfo.stage = stage;
		shaderInfo.module = shader;
		shaderInfo.pName = "main";

		return shaderInfo;
	}

	VkPipelineInputAssemblyStateCreateInfo getInputAssembly()
	{
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};

		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		return inputAssemblyInfo;
	}

	VkPipelineViewportStateCreateInfo getViewportStateInfo()
	{
		VkPipelineViewportStateCreateInfo viewportStateInfo{};

		viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateInfo.viewportCount = 1;
		viewportStateInfo.scissorCount = 1;

		return viewportStateInfo;
	}

	VkPipelineRasterizationStateCreateInfo getRasterizerInfo()
	{
		VkPipelineRasterizationStateCreateInfo rasterizerInfo{};

		rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizerInfo.depthClampEnable = VK_FALSE;
		rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizerInfo.lineWidth = 1.0f;
		rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizerInfo.depthBiasEnable = VK_FALSE;

		return rasterizerInfo;
	}

	VkPipelineColorBlendAttachmentState getBlendAttachmentState()
	{
		VkPipelineColorBlendAttachmentState colorBlendAttachmentState{};

		colorBlendAttachmentState.colorWriteMask =
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
				VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachmentState.blendEnable = VK_FALSE;

		return colorBlendAttachmentState;
	}

	VkPipelineMultisampleStateCreateInfo getMultisamplingInfo()
	{
		VkPipelineMultisampleStateCreateInfo multisamplingInfo{};

		multisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisamplingInfo.sampleShadingEnable = VK_FALSE;
		multisamplingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		return multisamplingInfo;
	}

	VkPipelineColorBlendStateCreateInfo getColorBlendInfo()
	{
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		return colorBlending;
	}

	std::vector<VkDynamicState> getDynamicStates()
	{
		std::vector<VkDynamicState> dynamicStates = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
		};

		return dynamicStates;
	}
}

namespace Animation
{
	ShaderVulkan::ShaderVulkan()
	{
		const LogicalDevice &device = LogicalDevice::getInstance();

		createPipelineLayout(device);
		createShader(device);
		setUniforms();
	}

	ShaderVulkan::~ShaderVulkan()
	{
		VkDevice device = LogicalDevice::getInstance().getDevice();
		vkDestroyPipeline(device, m_GraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);
	}

	void ShaderVulkan::startShader() const
	{
		RendererVulkan *renderer = getRenderer();
		VkCommandBuffer commandBuffer = renderer->getCurrentCommandBuffer();

		beginCommandBuffer(commandBuffer);
		beginRenderPass(renderer, commandBuffer);
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
		setViewport(commandBuffer, renderer->getSwapChain().getExtent());
		setScissor(commandBuffer, renderer->getSwapChain().getExtent());

		bindUniforms(commandBuffer);
	}

	void ShaderVulkan::stopShader() const
	{
		RendererVulkan *renderer = getRenderer();
		VkCommandBuffer commandBuffer = renderer->getCurrentCommandBuffer();

		vkCmdEndRenderPass(commandBuffer);
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			spdlog::error("Failed to record command buffer.");
			std::exit(EXIT_FAILURE);
		}
	}

	void ShaderVulkan::createShader(const LogicalDevice &device)
	{
		const RenderPass &renderPass = getRenderPass();

		VkShaderModule vertexShader = createShader(device, vertexShader_spv, vertexShader_spv_len);
		VkShaderModule fragmentShader = createShader(device, fragmentShader_spv, fragmentShader_spv_len);

		VkPipelineShaderStageCreateInfo vertexInfo = createShaderInfo(VK_SHADER_STAGE_VERTEX_BIT, vertexShader);
		VkPipelineShaderStageCreateInfo fragmentInfo = createShaderInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShader);

		VkPipelineInputAssemblyStateCreateInfo inputAssembly = getInputAssembly();
		VkPipelineViewportStateCreateInfo viewportState = getViewportStateInfo();
		VkPipelineRasterizationStateCreateInfo rasterizerInfo = getRasterizerInfo();
		VkPipelineMultisampleStateCreateInfo multisamplingInfo = getMultisamplingInfo();

		VkPipelineColorBlendStateCreateInfo colorBlendInfo = getColorBlendInfo();
		VkPipelineColorBlendAttachmentState blendAttachmentState = getBlendAttachmentState();
		colorBlendInfo.pAttachments = &blendAttachmentState;

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		VkVertexInputBindingDescription bindingDescription = getBindingDescription();
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = getAttributeDescription();
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkPipelineDynamicStateCreateInfo dynamicState{};
		std::vector<VkDynamicState> dynamicStates = getDynamicStates();
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = dynamicStates.size();
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineShaderStageCreateInfo shaderStages[] = {vertexInfo, fragmentInfo};

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizerInfo;
		pipelineInfo.pMultisampleState = &multisamplingInfo;
		pipelineInfo.pColorBlendState = &colorBlendInfo;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.renderPass = renderPass.getRenderPass();
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		createPipeline(device, pipelineInfo);

		vkDestroyShaderModule(device.getDevice(), fragmentShader, nullptr);
		vkDestroyShaderModule(device.getDevice(), vertexShader, nullptr);
	}

	void ShaderVulkan::createPipeline(const LogicalDevice &device, const VkGraphicsPipelineCreateInfo &pipelineInfo)
	{
		if (vkCreateGraphicsPipelines(device.getDevice(), VK_NULL_HANDLE, 1,
									  &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
		{
			spdlog::error("Failed to create graphics pipeline.");
			std::exit(EXIT_FAILURE);
		}

		assert(m_GraphicsPipeline != VK_NULL_HANDLE);
	}

	void ShaderVulkan::createPipelineLayout(const LogicalDevice &device)
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pushConstantRangeCount = 0;

		if (vkCreatePipelineLayout(device.getDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
		{
			spdlog::error("Failed to create pipeline layout.");
			std::exit(EXIT_FAILURE);
		}

		assert(m_PipelineLayout != VK_NULL_HANDLE);
	}

	VkShaderModule ShaderVulkan::createShader(const LogicalDevice &device, const std::uint8_t *data, std::uint32_t size)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = size;
		createInfo.pCode = reinterpret_cast<const uint32_t *>(data);

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(device.getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			spdlog::error("Failed to create shader module.");
			std::exit(EXIT_FAILURE);
		}

		return shaderModule;
	}

	VkVertexInputBindingDescription ShaderVulkan::getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		// TODO: do not hardcode this 3: This is data size from vertex object
		bindingDescription.stride = 3 * sizeof(float);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	std::vector<VkVertexInputAttributeDescription> ShaderVulkan::getAttributeDescription()
	{
		constexpr int numberOfAttributeDescriptions = 1;
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(numberOfAttributeDescriptions);

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = 0;

		return attributeDescriptions;
	}

	RendererVulkan *ShaderVulkan::getRenderer()
	{
		std::unique_ptr<Renderer> &renderer = Renderer::getRenderer();
		assert(renderer);

		auto *vulkanRenderer = dynamic_cast<RendererVulkan *>(renderer.get());
		assert(vulkanRenderer);

		return vulkanRenderer;
	}

	const RenderPass &ShaderVulkan::getRenderPass()
	{
		return getRenderer()->getRenderPass();
	}

	void ShaderVulkan::beginCommandBuffer(VkCommandBuffer commandBuffer)
	{
		VkCommandBufferBeginInfo beginCommandBuffer{.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

		if (vkBeginCommandBuffer(commandBuffer, &beginCommandBuffer) != VK_SUCCESS)
		{
			spdlog::error("Failed to begin command buffer.");
			std::exit(EXIT_FAILURE);
		}
	}

	void ShaderVulkan::beginRenderPass(const RendererVulkan *renderer, VkCommandBuffer commandBuffer)
	{
		VkClearValue clearColor = renderer->getClearColor();
		VkRenderPassBeginInfo renderPassInfo{};

		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderer->getRenderPass().getRenderPass();
		renderPassInfo.framebuffer = renderer->getCurrentFramebuffer().getFramebuffer();
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = renderer->getSwapChain().getExtent();
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void ShaderVulkan::setViewport(VkCommandBuffer commandBuffer, VkExtent2D extent)
	{
		VkViewport viewport{};

		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(extent.width);
		viewport.height = static_cast<float>(extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	}

	void ShaderVulkan::setScissor(VkCommandBuffer commandBuffer, VkExtent2D extent)
	{
		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent = extent;

		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void ShaderVulkan::setUniforms()
	{
		addUniform(std::make_unique<UniformVulkan>(0));
	}

	void ShaderVulkan::bindUniforms(VkCommandBuffer commandBuffer) const
	{
		for (const auto &uniform: m_Uniforms)
		{
			assert(uniform);
			const auto *uniformVulkan = dynamic_cast<UniformVulkan *>(uniform.get());
			assert(uniformVulkan);

			uniformVulkan->bindDescriptorSet(commandBuffer);
		}
	}
}