#ifndef SKELETALANIMATION_MESHDATA_H
#define SKELETALANIMATION_MESHDATA_H

#include <cstdint>

#include "VertexArrayObject.h"

namespace Animation
{
	class MeshData
	{
	private:
		static constexpr int DATA_SIZE = 3;

		std::uint64_t m_VertexCount{};
		std::unique_ptr<VertexArrayObject> m_VertexArrayObject;

	public:
		// TODO: create functions that will parse the mesh using assimp library

		// TODO: remove this constructor
		MeshData(const std::vector<float> &vertices, const std::vector<std::uint32_t> &indices);

		[[nodiscard]] std::uint64_t getVertexCount() const;

		void bindMesh() const;

		void unbindMesh() const;
	};
}

#endif //SKELETALANIMATION_MESHDATA_H
