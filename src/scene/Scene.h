#ifndef SKELETALANIMATION_SCENE_H
#define SKELETALANIMATION_SCENE_H

#include <memory>
#include <filesystem>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "Animation.h"
#include "Mesh.h"

namespace Animation
{
	class Scene final
	{
	private:
		std::unique_ptr<class Renderer> &m_Renderer;
		std::filesystem::path m_ScenePath;

		std::unique_ptr<Assimp::Importer> m_Importer;
		const aiScene *m_Scene{};

		std::unique_ptr<Animation> m_Animation;
		std::vector<Mesh> m_Meshes;

	public:
		static Scene createScene(const std::filesystem::path &sceneFile);

		void render();

		void updateAnimation();

	private:
		explicit Scene(std::filesystem::path scenePath);

		void parseScene();

		void parseMeshes(const std::vector<std::shared_ptr<class Material>> &materials);

		[[nodiscard]] std::vector<std::shared_ptr<class Material>> parseMaterials() const;
	};
}

#endif //SKELETALANIMATION_SCENE_H
