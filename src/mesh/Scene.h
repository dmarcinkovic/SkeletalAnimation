#ifndef SKELETALANIMATION_SCENE_H
#define SKELETALANIMATION_SCENE_H

#include <memory>
#include <filesystem>
#include <assimp/scene.h>

namespace Animation
{
	class Scene final
	{
	private:
		std::unique_ptr<class Renderer> &m_Renderer;
		std::filesystem::path m_ScenePath;

	public:
		static void createScene(const std::filesystem::path &sceneFile);

	private:
		explicit Scene(std::filesystem::path scenePath);

		void parseScene(const aiScene *scene);

		void parseMeshes(const aiScene *scene, const std::vector<std::shared_ptr<class Material>> &materials);

		std::vector<std::shared_ptr<class Material>> parseMaterials(const aiScene *scene) const;
	};
}

#endif //SKELETALANIMATION_SCENE_H
