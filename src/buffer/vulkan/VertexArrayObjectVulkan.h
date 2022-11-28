#ifndef SKELETALANIMATION_VERTEXARRAYOBJECTVULKAN_H
#define SKELETALANIMATION_VERTEXARRAYOBJECTVULKAN_H

#include <vulkan/vulkan.h>

#include "VertexArrayObject.h"

namespace Animation
{
	class VertexArrayObjectVulkan final : public VertexArrayObject
	{
	public:
		VertexArrayObjectVulkan();

		~VertexArrayObjectVulkan() override = default;

		void bind() const override;

		void unbind() const override;

		void storeFloatData(int attributeIndex, std::unique_ptr<VertexBufferObject> vertexBuffer) override;

		void storeIntData(int attributeIndex, std::unique_ptr<VertexBufferObject> vertexBuffer) override;

		void setIndexBufferData(const std::vector<std::uint32_t> &indices) override;
	};
}

#endif //SKELETALANIMATION_VERTEXARRAYOBJECTVULKAN_H
