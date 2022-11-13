#ifndef SKELETALANIMATION_TEXTUREVULKAN_H
#define SKELETALANIMATION_TEXTUREVULKAN_H

#include <vulkan/vulkan.h>

#include "Texture.h"
#include "VulkanBuffer.h"
#include "LogicalDevice.h"
#include "UniformVulkan.h"

namespace Animation
{
	class TextureVulkan final : public Texture
	{
	private:
		VkDevice m_Device;
		VkImage m_TextureImage{};
		VkDeviceMemory m_TextureImageMemory{};

	public:
		explicit TextureVulkan(const std::filesystem::path &path);

		~TextureVulkan() override;

		void createTexture(const std::unique_ptr<UniformVulkan> &uniform);

	private:
		static VulkanBuffer getStagingBuffer(VkDeviceSize size);

		void createImage();

		void transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);

		void copyBufferToImage(VkBuffer buffer);

		void createTextureSampler(const std::unique_ptr<UniformVulkan> &uniform);

		void prepareImage(VkBuffer buffer);
	};
}

#endif //SKELETALANIMATION_TEXTUREVULKAN_H
