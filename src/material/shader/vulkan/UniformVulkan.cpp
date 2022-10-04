#include <spdlog/spdlog.h>
#include <glm/vec4.hpp>

#include "UniformVulkan.h"
#include "LogicalDevice.h"
#include "CommandPool.h"

namespace
{
	VkDescriptorSetLayoutBinding getLayoutBinding(std::uint32_t binding)
	{
		VkDescriptorSetLayoutBinding layoutBinding{};
		layoutBinding.binding = binding;
		layoutBinding.descriptorCount = 1;
		layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		layoutBinding.pImmutableSamplers = nullptr;
		layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		return layoutBinding;
	}
}

namespace Animation
{
	UniformVulkan::UniformVulkan(std::uint32_t binding)
			: Uniform(binding)
	{
		m_Device = LogicalDevice::getInstance().getDevice();

		createDescriptorLayout();
		createUniformBuffers();
		createDescriptorPool();
		createDescriptorSets();
	}

	UniformVulkan::~UniformVulkan()
	{
		vkDestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
	}

	void UniformVulkan::update()
	{
		glm::vec4 uniformData{0.4f, 0.4f, 1.0f, 1.0f};

		assert(m_CurrentFrame < m_UniformBuffers.size());
		assert(m_UniformBuffers[m_CurrentFrame]);
		VkDeviceMemory memory = m_UniformBuffers[m_CurrentFrame]->getBufferMemory();

		void *data;
		vkMapMemory(m_Device, memory, 0, sizeof(uniformData), 0, &data);
		std::memcpy(data, &uniformData, sizeof(uniformData));
		vkUnmapMemory(m_Device, memory);

		++m_CurrentFrame;
		m_CurrentFrame %= CommandPool::size();
	}

	void UniformVulkan::createDescriptorLayout()
	{
		VkDescriptorSetLayoutBinding layoutBinding = getLayoutBinding(m_Binding);

		VkDescriptorSetLayoutCreateInfo layout{};
		layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layout.bindingCount = 1;
		layout.pBindings = &layoutBinding;

		if (vkCreateDescriptorSetLayout(m_Device, &layout, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
		{
			spdlog::error("Failed to create descriptor set layout.");
			std::exit(EXIT_FAILURE);
		}

		assert(m_DescriptorSetLayout != VK_NULL_HANDLE);
	}

	void UniformVulkan::createUniformBuffers()
	{
		VkDeviceSize bufferSize = sizeof(glm::vec4);

		const int numberOfUniformBuffers = CommandPool::size();
		for (int i = 0; i < numberOfUniformBuffers; ++i)
		{
			VkBufferUsageFlags usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			VkMemoryPropertyFlags flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			m_UniformBuffers.emplace_back(std::make_unique<VulkanBuffer>(bufferSize, usage, flags));
		}
	}

	void UniformVulkan::createDescriptorPool()
	{
		VkDescriptorPoolSize poolSize = getDescriptorPoolSize();

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = static_cast<uint32_t>(CommandPool::size());

		if (vkCreateDescriptorPool(m_Device, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
		{
			spdlog::error("Failed to create descriptor pool.");
			std::exit(EXIT_FAILURE);
		}

		assert(m_DescriptorPool != VK_NULL_HANDLE);
	}

	VkDescriptorPoolSize UniformVulkan::getDescriptorPoolSize()
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = static_cast<uint32_t>(Animation::CommandPool::size());

		return poolSize;
	}

	void UniformVulkan::createDescriptorSets()
	{
		const int numberOfUniformBuffers = CommandPool::size();
		std::vector<VkDescriptorSetLayout> layouts(numberOfUniformBuffers, m_DescriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(numberOfUniformBuffers);
		allocInfo.pSetLayouts = layouts.data();

		m_DescriptorSets.resize(numberOfUniformBuffers);
		allocateDescriptorSets(allocInfo);
		updateDescriptorSets();
	}

	void UniformVulkan::bindDescriptorSet(VkCommandBuffer commandBuffer) const
	{
		VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		assert(m_CurrentFrame < m_DescriptorSets.size());
		VkDescriptorSet descriptorSet = m_DescriptorSets[m_CurrentFrame];
		vkCmdBindDescriptorSets(commandBuffer, bindPoint, VK_NULL_HANDLE, 0, 1, &descriptorSet, 0, nullptr);
	}

	void UniformVulkan::updateDescriptorSets()
	{
		for (size_t i = 0; i < m_UniformBuffers.size(); i++)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = m_UniformBuffers[i]->getBuffer();
			bufferInfo.offset = 0;
			// TODO: change
			bufferInfo.range = sizeof(glm::vec4);

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;

			vkUpdateDescriptorSets(m_Device, 1, &descriptorWrite, 0, nullptr);
		}
	}

	void UniformVulkan::allocateDescriptorSets(const VkDescriptorSetAllocateInfo &info)
	{
		if (vkAllocateDescriptorSets(m_Device, &info, m_DescriptorSets.data()) != VK_SUCCESS)
		{
			spdlog::error("Failed to allocate descriptor sets.");
			std::exit(EXIT_FAILURE);
		}
	}
}