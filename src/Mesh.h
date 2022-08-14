#ifndef SKELETALANIMATION_MESH_H
#define SKELETALANIMATION_MESH_H

#include "Material.h"
#include "MeshData.h"

namespace Animation
{
	class Mesh final
	{
	private:
		MeshData m_MeshData;
		Material m_Material;

	public:
		Mesh(MeshData meshData, Material material);

		void render() const;
	};
}

#endif //SKELETALANIMATION_MESH_H
