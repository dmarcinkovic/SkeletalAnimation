#include <assimp/Importer.hpp>
#include <utility>
#include <assimp/postprocess.h>
#include <spdlog/spdlog.h>

#include "Scene.h"
#include "Renderer.h"

namespace Animation
{
	Scene::Scene(std::filesystem::path scenePath)
			: m_Renderer(Renderer::getRenderer()), m_ScenePath(std::move(scenePath))
	{
	}

	void Scene::createScene(const std::filesystem::path &sceneFile)
	{
		assert(std::filesystem::exists(sceneFile));

		Assimp::Importer importer;
		const std::uint32_t flags =
				aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals;
		const aiScene *aiScene = importer.ReadFile(sceneFile.c_str(), flags);

		if (!aiScene)
		{
			spdlog::error("Failed to parse '{}' file: {}", sceneFile.string(), importer.GetErrorString());
			std::exit(EXIT_FAILURE);
		}

		Scene scene(sceneFile);
		scene.parseScene(aiScene);
	}

	void Scene::parseScene(const aiScene *scene)
	{
		const std::vector<std::shared_ptr<Material>> materials = parseMaterials(scene);
		assert(!materials.empty());

		parseMeshes(scene, materials);
	}

	void Scene::parseMeshes(const aiScene *scene, const std::vector<std::shared_ptr<Material>> &materials)
	{
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh *mesh = scene->mMeshes[i];
			assert(mesh);

			MeshData meshData(mesh);
			assert(mesh->mMaterialIndex < materials.size());
			const std::shared_ptr<Material> &material = materials[mesh->mMaterialIndex];

			m_Renderer->addMesh(Mesh(std::move(meshData), material));
		}
	}

	std::vector<std::shared_ptr<Material>> Scene::parseMaterials(const aiScene *scene) const
	{
		static std::shared_ptr<Material> defaultMaterial = std::make_shared<Material>();

		std::vector<std::shared_ptr<Material>> materials;
		if (scene->HasMaterials())
		{
			for (int i = 0; i < scene->mNumMaterials; ++i)
			{
				const aiMaterial *material = scene->mMaterials[i];
				assert(material);

				materials.emplace_back(std::make_shared<Material>(m_ScenePath, scene, material));
			}
		} else
		{
			materials.emplace_back(defaultMaterial);
		}

		return materials;
	}
}