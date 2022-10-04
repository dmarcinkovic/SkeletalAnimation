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
		VkDevice m_Device{};
		VkDescriptorSetLayout m_DescriptorSetLayout{};
		VkDescriptorPool m_DescriptorPool{};

		std::vector<std::unique_ptr<VulkanBuffer>> m_UniformBuffers;
		std::vector<VkDescriptorSet> m_DescriptorSets;

		std::uint32_t m_CurrentFrame{};

	public:
		explicit UniformVulkan(std::uint32_t binding);

		~UniformVulkan() override;

		void update() override;

		void bindDescriptorSet(VkCommandBuffer commandBuffer) const;

	private:
		void createDescriptorLayout();

		void createUniformBuffers();

		void createDescriptorPool();

		static VkDescriptorPoolSize getDescriptorPoolSize();

		void createDescriptorSets();

		void updateDescriptorSets();

		void allocateDescriptorSets(const VkDescriptorSetAllocateInfo& info);
	};
}

#endif //SKELETALANIMATION_UNIFORMVULKAN_H
