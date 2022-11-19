#include <cassert>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <spdlog/spdlog.h>

#include "MeshData.h"
#include "Renderer.h"

namespace Animation
{
	MeshData::MeshData(const aiMesh *mesh)
			: m_VertexArrayObject(Renderer::getRenderer()->createVertexArrayObject())
	{
		const std::unique_ptr<Renderer> &renderer = Renderer::getRenderer();

		parseIndices(mesh, renderer);
		parseVertices(mesh, renderer);
		parseTextureCoordinates(mesh, renderer);
		parseNormals(mesh, renderer);
	}

	std::uint64_t MeshData::getVertexCount() const
	{
		return m_IndicesCount;
	}

	void MeshData::bindMesh() const
	{
		assert(m_VertexArrayObject);
		m_VertexArrayObject->bind();
	}

	void MeshData::unbindMesh() const
	{
		assert(m_VertexArrayObject);
		m_VertexArrayObject->unbind();
	}

	void MeshData::parseVertices(const aiMesh *mesh, const std::unique_ptr<Renderer> &renderer)
	{
		assert(mesh->HasPositions());
		std::vector<float> vertices;
		vertices.reserve(VERTEX_DATA_SIZE * mesh->mNumVertices);

		for (int i = 0; i < mesh->mNumVertices; ++i)
		{
			const aiVector3D &vertex = mesh->mVertices[i];
			vertices.emplace_back(vertex.x);
			vertices.emplace_back(vertex.y);
			vertices.emplace_back(vertex.z);
		}

		std::unique_ptr<VertexBufferObject> positionData = renderer->createVertexBufferObject();
		positionData->storeFloatData(0, vertices, VERTEX_DATA_SIZE);
		m_VertexArrayObject->storeData(0, std::move(positionData));
	}

	void MeshData::parseTextureCoordinates(const aiMesh *mesh, const std::unique_ptr<Renderer> &renderer)
	{
		if (mesh->HasTextureCoords(0))
		{
			std::vector<float> uvs;
			uvs.reserve(UV_DATA_SIZE * mesh->mNumVertices);

			for (int i = 0; i < mesh->mNumVertices; ++i)
			{
				const aiVector3D &uv = mesh->mTextureCoords[0][i];
				uvs.emplace_back(uv.x);
				uvs.emplace_back(uv.y);
			}

			std::unique_ptr<VertexBufferObject> textureData = renderer->createVertexBufferObject();
			textureData->storeFloatData(1, uvs, UV_DATA_SIZE);
			m_VertexArrayObject->storeData(1, std::move(textureData));
		}
	}

	void MeshData::parseNormals(const aiMesh *mesh, const std::unique_ptr<Renderer> &renderer)
	{
		if (mesh->HasNormals())
		{
			std::vector<float> normals;
			normals.reserve(NORMALS_DATA_SIZE * mesh->mNumVertices);

			for (int i = 0; i < mesh->mNumVertices; ++i)
			{
				const aiVector3D &normal = mesh->mNormals[i];
				normals.emplace_back(normal.x);
				normals.emplace_back(normal.y);
				normals.emplace_back(normal.z);
			}

			std::unique_ptr<VertexBufferObject> normalsData = renderer->createVertexBufferObject();
			normalsData->storeFloatData(2, normals, NORMALS_DATA_SIZE);
			m_VertexArrayObject->storeData(2, std::move(normalsData));
		}
	}

	void MeshData::parseIndices(const aiMesh *mesh, const std::unique_ptr<Renderer> &renderer)
	{
		assert(mesh->HasFaces());

		m_IndicesCount = 3 * mesh->mNumFaces;
		std::vector<std::uint32_t> indices;
		indices.reserve(m_IndicesCount);

		for (int i = 0; i < mesh->mNumFaces; ++i)
		{
			const aiFace &face = mesh->mFaces[i];
			assert(face.mNumIndices == 3);

			indices.emplace_back(face.mIndices[0]);
			indices.emplace_back(face.mIndices[1]);
			indices.emplace_back(face.mIndices[2]);
		}
		m_VertexArrayObject->setIndexBufferData(indices);
	}
}