#ifndef SKELETALANIMATION_VERTEXARRAYOBJECTVULKAN_H
#define SKELETALANIMATION_VERTEXARRAYOBJECTVULKAN_H

#include <vulkan/vulkan.h>

#include "VertexArrayObject.h"

namespace Animation
{
	class VertexArrayObjectVulkan final : public VertexArrayObject
	{
	private:
		VkBuffer m_VertexBuffer{};

	public:
		VertexArrayObjectVulkan();

		~VertexArrayObjectVulkan() override = default;

		void bind() const override;

		void unbind() const override;

		void storeData(int attributeIndex, std::unique_ptr<VertexBufferObject> vertexBufferObject) override;

		void setIndexBufferData(const std::vector<std::uint32_t> &indices) override;
	};
}

#endif //SKELETALANIMATION_VERTEXARRAYOBJECTVULKAN_H
