#ifndef SKELETALANIMATION_VERTEXBUFFEROBJECTVULKAN_H
#define SKELETALANIMATION_VERTEXBUFFEROBJECTVULKAN_H

#include <vulkan/vulkan.h>
#include <memory>

#include "VertexBufferObject.h"
#include "RendererVulkan.h"
#include "VulkanBuffer.h"

namespace Animation
{
	class VertexBufferObjectVulkan final : public VertexBufferObject
	{
	private:
		std::unique_ptr<VulkanBuffer> m_VertexBuffer;

	public:
		~VertexBufferObjectVulkan() override = default;

		void bind() const override;

		void unbind() const override;

		void storeFloatData(std::uint32_t binding, const std::vector<float> &data, int dataSize) override;

	private:
		static VulkanBuffer getStagingBuffer(VkDeviceSize bufferSize);

		void createVertexBuffer(VkDeviceSize bufferSize);
	};
}

#endif //SKELETALANIMATION_VERTEXBUFFEROBJECTVULKAN_H
