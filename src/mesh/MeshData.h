#ifndef SKELETALANIMATION_MESHDATA_H
#define SKELETALANIMATION_MESHDATA_H

#include <cstdint>
#include <filesystem>
#include <assimp/scene.h>

#include "VertexArrayObject.h"

namespace Animation
{
	class MeshData
	{
	private:
		static constexpr int VERTEX_DATA_SIZE = 3;
		static constexpr int NORMALS_DATA_SIZE = 3;
		static constexpr int UV_DATA_SIZE = 2;
		static constexpr int NUMBER_OF_BONES_PER_VERTEX = AI_LMW_MAX_WEIGHTS;

		std::uint64_t m_IndicesCount{};
		std::unique_ptr<VertexArrayObject> m_VertexArrayObject{};

	public:
		explicit MeshData(const class aiMesh *mesh);

		[[nodiscard]] std::uint64_t getVertexCount() const;

		void bindMesh() const;

		void unbindMesh() const;

	private:
		void parseVertices(const aiMesh *mesh, const std::unique_ptr<class Renderer> &renderer);

		void parseTextureCoordinates(const aiMesh *mesh, const std::unique_ptr<class Renderer> &renderer);

		void parseNormals(const aiMesh *mesh, const std::unique_ptr<class Renderer> &renderer);

		void parseIndices(const aiMesh *mesh, const std::unique_ptr<class Renderer> &renderer);

		void parseBones(const aiMesh *mesh, const std::unique_ptr<class Renderer> &renderer);
	};
}

#endif //SKELETALANIMATION_MESHDATA_H
