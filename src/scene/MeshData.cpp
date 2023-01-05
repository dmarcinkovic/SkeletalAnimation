#include <cassert>
#include <assimp/Importer.hpp>
#include <spdlog/spdlog.h>

#include "MeshData.h"
#include "Renderer.h"
#include "Bone.h"

namespace
{
	void parseBone(const aiBone *bone, int boneId, std::vector<std::vector<float>> &weights,
				   std::vector<std::vector<int>> &ids)
	{
		for (int j = 0; j < bone->mNumWeights; ++j)
		{
			aiVertexWeight weight = bone->mWeights[j];

			unsigned int vertexIndex = weight.mVertexId;
			assert(vertexIndex < weights.size());
			assert(vertexIndex < ids.size());

			weights[vertexIndex].emplace_back(weight.mWeight);
			ids[vertexIndex].emplace_back(boneId);

			assert(weights[vertexIndex].size() <= AI_LMW_MAX_WEIGHTS);
			assert(ids[vertexIndex].size() <= AI_LMW_MAX_WEIGHTS);
		}
	}

	std::vector<float> flattenBoneWeights(std::vector<std::vector<float>> boneWeights)
	{
		std::vector<float> result;
		result.reserve(AI_LMW_MAX_WEIGHTS * boneWeights.size());

		for (auto &weights: boneWeights)
		{
			weights.resize(AI_LMW_MAX_WEIGHTS);
			for (float weight: weights)
			{
				result.emplace_back(weight);
			}
		}

		return result;
	}

	std::vector<int> flattenBoneIndices(std::vector<std::vector<int>> boneIds)
	{
		std::vector<int> result;
		result.reserve(AI_LMW_MAX_WEIGHTS * boneIds.size());

		for (auto &ids: boneIds)
		{
			ids.resize(AI_LMW_MAX_WEIGHTS, -1);
			for (int id: ids)
			{
				result.emplace_back(id);
			}
		}

		return result;
	}
}

namespace Animation
{
	MeshData::MeshData(const aiMesh *mesh)
			: m_VertexArrayObject(Renderer::getRenderer()->createVertexArrayObject())
	{
		const std::unique_ptr<Renderer> &renderer = Renderer::getRenderer();

		parseIndices(mesh);
		parseVertices(mesh, renderer);
		parseTextureCoordinates(mesh, renderer);
		parseNormals(mesh, renderer);
		parseBones(mesh, renderer);
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
		m_VertexArrayObject->storeFloatData(0, std::move(positionData));
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
			m_VertexArrayObject->storeFloatData(1, std::move(textureData));
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
			m_VertexArrayObject->storeFloatData(2, std::move(normalsData));
		}
	}

	void MeshData::parseIndices(const aiMesh *mesh)
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

	void MeshData::parseBones(const aiMesh *mesh, const std::unique_ptr<Renderer> &renderer)
	{
		std::vector<float> weights;
		std::vector<int> ids;

		if (mesh->HasBones())
		{
			std::vector<std::vector<float>> boneWeights(mesh->mNumVertices);
			std::vector<std::vector<int>> boneIds(mesh->mNumVertices);

			for (int i = 0; i < mesh->mNumBones; ++i)
			{
				const aiBone *bone = mesh->mBones[i];
				assert(bone);

				const std::string boneName = bone->mName.C_Str();
				Bone::mapBone(boneName);
				Bone::setBoneOffset(boneName, bone->mOffsetMatrix);
				int boneId = Bone::getBoneIndex(boneName);

				parseBone(bone, boneId, boneWeights, boneIds);
			}

			weights = flattenBoneWeights(std::move(boneWeights));
			ids = flattenBoneIndices(std::move(boneIds));
		} else
		{
			weights.resize(mesh->mNumVertices * NUMBER_OF_BONES_PER_VERTEX);
			ids.resize(mesh->mNumVertices * NUMBER_OF_BONES_PER_VERTEX, -1);
		}

		std::unique_ptr<VertexBufferObject> boneWeightsData = renderer->createVertexBufferObject();
		boneWeightsData->storeFloatData(3, weights, NUMBER_OF_BONES_PER_VERTEX);
		m_VertexArrayObject->storeFloatData(3, std::move(boneWeightsData));

		std::unique_ptr<VertexBufferObject> boneIdsData = renderer->createVertexBufferObject();
		boneIdsData->storeIntData(4, ids, NUMBER_OF_BONES_PER_VERTEX);
		m_VertexArrayObject->storeIntData(4, std::move(boneIdsData));
	}
}