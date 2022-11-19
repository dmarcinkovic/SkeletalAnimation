#ifndef SKELETALANIMATION_MESHDATA_H
#define SKELETALANIMATION_MESHDATA_H

#include <cstdint>
#include <filesystem>

#include "VertexArrayObject.h"

namespace Animation
{
	class MeshData
	{
	private:
		static constexpr int VERTEX_DATA_SIZE = 3;
		static constexpr int NORMALS_DATA_SIZE = 3;
		static constexpr int UV_DATA_SIZE = 2;

		std::uint64_t m_IndicesCount{};
		std::unique_ptr<VertexArrayObject> m_VertexArrayObject{};

	public:
		explicit MeshData(const class aiMesh *mesh);

		[[nodiscard]] std::uint64_t getVertexCount() const;

		void bindMesh() const;

		void unbindMesh() const;

	private:
		void parseVertices(const class aiMesh *mesh, const std::unique_ptr<class Renderer> &renderer);

		void parseTextureCoordinates(const class aiMesh *mesh, const std::unique_ptr<class Renderer> &renderer);

		void parseNormals(const class aiMesh *mesh, const std::unique_ptr<class Renderer> &renderer);

		void parseIndices(const class aiMesh *mesh, const std::unique_ptr<class Renderer> &renderer);
	};
}

#endif //SKELETALANIMATION_MESHDATA_H
