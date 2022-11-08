#include <spdlog/spdlog.h>
#include <glm/vec4.hpp>

#include "UniformVulkan.h"
#include "LogicalDevice.h"
#include "CommandPool.h"

namespace
{
	VkImageViewCreateInfo getImageViewInfo(VkImage image)
	{
		// TODO: this is already in image view class: reuse
		VkImageViewCreateInfo imageViewInfo{};
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image = image;
		imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;

		return imageViewInfo;
	}

	VkSamplerCreateInfo getSamplerInfo(VkPhysicalDeviceProperties properties)
	{
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

		return samplerInfo;
	}
}

namespace Animation
{
	UniformVulkan::UniformVulkan(std::uint32_t uniform, std::uint32_t sampler, VkDescriptorSetLayout layout)
			: Uniform(uniform, sampler)
	{
		m_Device = LogicalDevice::getInstance().getDevice();

		createUniformBuffers();
		createDescriptorPool();
		createDescriptorSets(layout);
	}

	UniformVulkan::~UniformVulkan()
	{
		vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);

		vkDestroySampler(m_Device, m_TextureSampler, nullptr);
		vkDestroyImageView(m_Device, m_TextureImageView, nullptr);
	}

	void UniformVulkan::bind() const
	{
	}

	void UniformVulkan::update(const UniformData &uniformData)
	{
		assert(m_CurrentFrame < m_UniformBuffers.size());
		assert(m_UniformBuffers[m_CurrentFrame]);
		VkDeviceMemory memory = m_UniformBuffers[m_CurrentFrame]->getBufferMemory();

		void *data;
		vkMapMemory(m_Device, memory, 0, sizeof(UniformData), 0, &data);
		std::memcpy(data, &uniformData, sizeof(UniformData));
		vkUnmapMemory(m_Device, memory);

		++m_CurrentFrame;
		m_CurrentFrame %= CommandPool::size();
	}

	void UniformVulkan::unbind() const
	{

	}

	void UniformVulkan::createUniformBuffers()
	{
		VkDeviceSize bufferSize = sizeof(UniformData);

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
		VkDescriptorPoolSize uniformPoolSize = getDescriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		VkDescriptorPoolSize samplerPoolSize = getDescriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

		std::array<VkDescriptorPoolSize, 2> poolSizes{uniformPoolSize, samplerPoolSize};

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = poolSizes.size();
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<std::uint32_t>(CommandPool::size());

		if (vkCreateDescriptorPool(m_Device, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
		{
			spdlog::error("Failed to create descriptor pool.");
			std::exit(EXIT_FAILURE);
		}

		assert(m_DescriptorPool != VK_NULL_HANDLE);
	}

	VkDescriptorPoolSize UniformVulkan::getDescriptorPoolSize(VkDescriptorType type)
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type = type;
		poolSize.descriptorCount = static_cast<std::uint32_t>(Animation::CommandPool::size());

		return poolSize;
	}

	void UniformVulkan::createDescriptorSets(VkDescriptorSetLayout descriptorSetLayout)
	{
		const int numberOfUniformBuffers = CommandPool::size();
		std::vector<VkDescriptorSetLayout> layouts(numberOfUniformBuffers, descriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = static_cast<std::uint32_t>(numberOfUniformBuffers);
		allocInfo.pSetLayouts = layouts.data();

		m_DescriptorSets.resize(numberOfUniformBuffers);
		allocateDescriptorSets(allocInfo);
	}

	void UniformVulkan::bindDescriptorSet(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) const
	{
		VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		assert(m_CurrentFrame < m_DescriptorSets.size());
		VkDescriptorSet descriptorSet = m_DescriptorSets[m_CurrentFrame];
		assert(pipelineLayout != VK_NULL_HANDLE);
		vkCmdBindDescriptorSets(commandBuffer, bindPoint, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
	}

	void UniformVulkan::updateDescriptorSets()
	{
		assert(m_TextureImageView != VK_NULL_HANDLE);
		assert(m_TextureSampler != VK_NULL_HANDLE);

		for (size_t i = 0; i < m_UniformBuffers.size(); i++)
		{
			VkDescriptorBufferInfo uniformInfo{};
			uniformInfo.buffer = m_UniformBuffers[i]->getBuffer();
			uniformInfo.offset = 0;
			uniformInfo.range = sizeof(UniformData);

			// TODO: move this to separate method
			VkDescriptorImageInfo samplerInfo{};
			samplerInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			samplerInfo.imageView = m_TextureImageView;
			samplerInfo.sampler = m_TextureSampler;

			VkWriteDescriptorSet uniformDescriptorWrite{};
			uniformDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			uniformDescriptorWrite.dstSet = m_DescriptorSets[i];
			uniformDescriptorWrite.dstBinding = 0;
			uniformDescriptorWrite.dstArrayElement = 0;
			uniformDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			uniformDescriptorWrite.descriptorCount = 1;
			uniformDescriptorWrite.pBufferInfo = &uniformInfo;

			VkWriteDescriptorSet samplerDescriptorWrite{};
			samplerDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			samplerDescriptorWrite.dstSet = m_DescriptorSets[i];
			samplerDescriptorWrite.dstBinding = 1;
			samplerDescriptorWrite.dstArrayElement = 0;
			samplerDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			samplerDescriptorWrite.descriptorCount = 1;
			samplerDescriptorWrite.pImageInfo = &samplerInfo;

			std::array<VkWriteDescriptorSet, 2> descriptors{uniformDescriptorWrite, samplerDescriptorWrite};

			vkUpdateDescriptorSets(m_Device, descriptors.size(), descriptors.data(), 0, nullptr);
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

	void UniformVulkan::createTextureImageView(VkImage textureImage)
	{
		VkImageViewCreateInfo imageViewInfo = getImageViewInfo(textureImage);

		if (vkCreateImageView(m_Device, &imageViewInfo, nullptr, &m_TextureImageView) != VK_SUCCESS)
		{
			spdlog::error("Failed to create texture image view.");
			std::exit(EXIT_FAILURE);
		}

		assert(m_TextureImageView != VK_NULL_HANDLE);
	}

	void UniformVulkan::createTextureSampler()
	{
		VkPhysicalDevice physicalDevice = LogicalDevice::getInstance().getPhysicalDevice();
		VkPhysicalDeviceProperties properties{};

		vkGetPhysicalDeviceProperties(physicalDevice, &properties);
		VkSamplerCreateInfo samplerInfo = getSamplerInfo(properties);

		if (vkCreateSampler(m_Device, &samplerInfo, nullptr, &m_TextureSampler) != VK_SUCCESS)
		{
			spdlog::error("Failed to create texture sampler.");
			std::exit(EXIT_FAILURE);
		}

		assert(m_TextureSampler != VK_NULL_HANDLE);
	}
}