#ifndef SKELETALANIMATION_UNIFORMVULKAN_H
#define SKELETALANIMATION_UNIFORMVULKAN_H

#include <vulkan/vulkan.h>

#include "Uniform.h"
#include "VulkanBuffer.h"

namespace Animation
{
	class UniformVulkan final : public Uniform
	{
	private:
		friend class TextureVulkan;

		VkDevice m_Device{};
		VkDescriptorSetLayout m_DescriptorSetLayout{};
		VkDescriptorPool m_DescriptorPool{};

		std::vector<std::unique_ptr<VulkanBuffer>> m_UniformBuffers;
		std::vector<VkDescriptorSet> m_DescriptorSets;

		VkImageView m_TextureImageView{};
		VkSampler m_TextureSampler{};

		std::uint32_t m_CurrentFrame{};

	public:
		UniformVulkan(std::uint32_t uniformBinding, std::uint32_t samplerBinding);

		~UniformVulkan() override;

		UniformVulkan(const UniformVulkan &) = delete;

		void bind() const override;

		void update() override;

		void unbind() const override;

		void bindDescriptorSet(VkCommandBuffer commandBuffer) const;

	private:
		void createDescriptorLayout();

		void createUniformBuffers();

		void createDescriptorPool();

		static VkDescriptorPoolSize getDescriptorPoolSize(VkDescriptorType type);

		void createDescriptorSets();

		void updateDescriptorSets();

		void allocateDescriptorSets(const VkDescriptorSetAllocateInfo &info);

		void createTextureImageView(VkImage textureImage);

		void createTextureSampler();
	};
}

#endif //SKELETALANIMATION_UNIFORMVULKAN_H
