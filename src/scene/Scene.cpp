#include <utility>
#include <assimp/postprocess.h>
#include <spdlog/spdlog.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Scene.h"
#include "Renderer.h"

namespace Animation
{
	Scene::Scene(std::filesystem::path scenePath)
			: m_Renderer(Renderer::getRenderer()), m_ScenePath(std::move(scenePath)),
			  m_Importer(std::make_unique<Assimp::Importer>())
	{
	}

	Scene Scene::createScene(const std::filesystem::path &sceneFile)
	{
		assert(std::filesystem::exists(sceneFile));
		Scene scene(sceneFile);

		const std::uint32_t flags =
				aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals |
				aiProcess_LimitBoneWeights;
		scene.m_Scene = scene.m_Importer->ReadFile(sceneFile.c_str(), flags);

		if (!scene.m_Scene)
		{
			spdlog::error("Failed to parse '{}' file: {}", sceneFile.string(), scene.m_Importer->GetErrorString());
			std::exit(EXIT_FAILURE);
		}

		scene.parseScene();
		return scene;
	}

	void Scene::parseScene()
	{
		const std::vector<std::shared_ptr<Material>> materials = parseMaterials();
		assert(!materials.empty());

		parseMeshes(materials);
		m_Animation = std::make_unique<Animation>(m_Scene);
	}

	void Scene::parseMeshes(const std::vector<std::shared_ptr<Material>> &materials)
	{
		for (int i = 0; i < m_Scene->mNumMeshes; ++i)
		{
			const aiMesh *mesh = m_Scene->mMeshes[i];
			assert(mesh);

			MeshData meshData(mesh);
			assert(mesh->mMaterialIndex < materials.size());
			const std::shared_ptr<Material> &material = materials[mesh->mMaterialIndex];

			m_Meshes.emplace_back(std::move(meshData), material);
		}
	}

	std::vector<std::shared_ptr<Material>> Scene::parseMaterials() const
	{
		static std::shared_ptr<Material> defaultMaterial = std::make_shared<Material>();

		std::vector<std::shared_ptr<Material>> materials;
		if (m_Scene->HasMaterials())
		{
			for (int i = 0; i < m_Scene->mNumMaterials; ++i)
			{
				const aiMaterial *material = m_Scene->mMaterials[i];
				assert(material);

				materials.emplace_back(std::make_shared<Material>(m_ScenePath, m_Scene, material));
			}
		} else
		{
			materials.emplace_back(defaultMaterial);
		}

		return materials;
	}

	void Scene::updateAnimation()
	{
		static auto lastFrame = std::chrono::high_resolution_clock::now();
		auto currentFrame = std::chrono::high_resolution_clock::now();

		const aiNode *rootNode = m_Scene->mRootNode;
		assert(rootNode);

		long frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(currentFrame - lastFrame).count();
		lastFrame = currentFrame;
		float durationInSeconds = static_cast<float>(frameDuration) / 1e6f;

		assert(m_Animation);
		m_Animation->play(rootNode, durationInSeconds);
	}

	void Scene::render()
	{
		m_Renderer->preRender();

		Uniform::UniformData uniformData{};
		const glm::vec4 lightColor{1, 1, 1, 1};
		const glm::vec3 cameraPosition{0, 0, 1};
		const glm::vec3 lightPosition{1, 0, 0};
		const glm::vec3 translation{0, -5, -30};
		const glm::vec3 scale{0.1f};
		const glm::quat rotation = glm::quat(glm::radians(glm::vec3{0, 45, 0}));
		const glm::mat4 modelMatrix = glm::translate(glm::mat4{1.0f}, translation) * glm::toMat4(rotation) *
									  glm::scale(glm::mat4{1.0f}, scale);
		const glm::mat4 viewMatrix = glm::lookAt(cameraPosition, glm::vec3{0}, glm::vec3{0, 1, 0});
		const glm::mat4 projectionMatrix = m_Renderer->getProjectionMatrix(45.0f, 0.1f, 100.0f);

		uniformData.projectionMatrix = projectionMatrix;
		uniformData.modelMatrix = modelMatrix;
		uniformData.viewMatrix = viewMatrix;
		uniformData.cameraPosition = cameraPosition;
		uniformData.lightColor = lightColor;
		uniformData.lightPosition = lightPosition;

		if (m_Animation->hasAnimation())
		{
			const std::vector<glm::mat4> &boneTransforms = m_Animation->getTransforms();
			for (int i = 0; i < boneTransforms.size(); ++i)
			{
				uniformData.boneTransforms[i] = boneTransforms[i];
			}
		}

		for (const Mesh &mesh: m_Meshes)
		{
			mesh.render(uniformData);
		}

		m_Renderer->postRender();
	}
}