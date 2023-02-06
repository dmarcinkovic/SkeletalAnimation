#ifndef SKELETALANIMATION_MESH_H
#define SKELETALANIMATION_MESH_H

#include <memory>

#include "Material.h"
#include "MeshData.h"

namespace Animation
{
	class Mesh final
	{
	private:
		MeshData m_MeshData;
		std::shared_ptr<Material> m_Material;

		glm::mat4 m_WorldTransform{1.0f};

	public:
		Mesh(MeshData meshData, std::shared_ptr<Material> material);

		void render(Uniform::UniformData uniformData) const;

		void setWorldTransform(const glm::mat4 &worldTransform);
	};
}

#endif //SKELETALANIMATION_MESH_H
