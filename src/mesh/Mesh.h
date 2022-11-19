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

	public:
		Mesh(MeshData meshData, std::shared_ptr<Material> material);

		void render() const;
	};
}

#endif //SKELETALANIMATION_MESH_H
