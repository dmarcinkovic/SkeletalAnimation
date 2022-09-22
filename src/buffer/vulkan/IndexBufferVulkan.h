#ifndef SKELETALANIMATION_INDEXBUFFERVULKAN_H
#define SKELETALANIMATION_INDEXBUFFERVULKAN_H

#include <vulkan/vulkan.h>
#include <memory>

#include "IndexBuffer.h"
#include "RendererVulkan.h"
#include "VulkanBuffer.h"

namespace Animation
{
	class IndexBufferVulkan final : public IndexBuffer
	{
	private:
		std::unique_ptr<VulkanBuffer> m_IndexBuffer;

	public:
		~IndexBufferVulkan() override = default;

		void bind() const override;

		void unbind() const override;

		void storeData(const std::vector<std::uint32_t> &indices) override;

	private:
		static VulkanBuffer getStagingBuffer(VkDeviceSize bufferSize);

		void createIndexBuffer(VkDeviceSize bufferSize);
	};
}

#endif //SKELETALANIMATION_INDEXBUFFERVULKAN_H
