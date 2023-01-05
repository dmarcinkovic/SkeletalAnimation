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
		parseCameras();
		parseLights();

		m_Animation = std::make_unique<Animation>(m_Scene);
	}

	void Scene::parseMeshes(const std::vector<std::shared_ptr<Material>> &materials)
	{
		spdlog::info("Found {} meshes.", m_Scene->mNumMeshes);

		for (int i = 0; i < m_Scene->mNumMeshes; ++i)
		{
			const aiMesh *mesh = m_Scene->mMeshes[i];
			assert(mesh);

			spdlog::info("Parse mesh '{}'.", mesh->mName.C_Str());

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
			spdlog::info("Found {} materials.", m_Scene->mNumMaterials);

			for (int i = 0; i < m_Scene->mNumMaterials; ++i)
			{
				const aiMaterial *material = m_Scene->mMaterials[i];
				assert(material);

				materials.emplace_back(std::make_shared<Material>(m_ScenePath, m_Scene, material));
			}
		} else
		{
			spdlog::info("No materials found. Default material will be used.");

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

		Uniform::UniformData uniformData = getUniformData();

		for (const Mesh &mesh: m_Meshes)
		{
			mesh.render(uniformData);
		}

		m_Renderer->postRender();
	}

	void Scene::parseCameras()
	{
		if (m_Scene->HasCameras())
		{
			const aiCamera *camera = m_Scene->mCameras[0];
			assert(camera);

			m_Camera = Camera(camera);
		}
	}

	void Scene::parseLights()
	{
		if (m_Scene->HasLights())
		{
			const aiLight *light = m_Scene->mLights[0];
			assert(light);

			m_Light = Light(light);
		}
	}

	Uniform::UniformData Scene::getUniformData() const
	{
		Uniform::UniformData uniformData{};

		static const glm::vec3 translation{0, -5, -30};
		static const glm::vec3 scale{0.1f};
		static const glm::quat rotation = glm::quat(glm::radians(glm::vec3{0, 45, 0}));
		static const glm::mat4 modelMatrix = glm::translate(glm::mat4{1.0f}, translation) * glm::toMat4(rotation) *
											 glm::scale(glm::mat4{1.0f}, scale);
		const glm::mat4 projectionMatrix = m_Renderer->getProjectionMatrix(m_Camera);

		uniformData.projectionMatrix = projectionMatrix;
		uniformData.modelMatrix = modelMatrix;
		uniformData.viewMatrix = m_Camera.getViewMatrix();
		uniformData.cameraPosition = m_Camera.getPosition();
		uniformData.lightColor = m_Light.getColor();
		uniformData.lightPosition = m_Light.getPosition();

		if (m_Animation->hasAnimation())
		{
			const std::vector<glm::mat4> &boneTransforms = m_Animation->getTransforms();
			for (int i = 0; i < boneTransforms.size(); ++i)
			{
				uniformData.boneTransforms[i] = boneTransforms[i];
			}
		}

		return uniformData;
	}
}