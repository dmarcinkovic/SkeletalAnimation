#ifndef SKELETALANIMATION_MATERIAL_H
#define SKELETALANIMATION_MATERIAL_H

#include <memory>
#include <assimp/scene.h>

#include "Shader.h"
#include "Texture.h"

namespace Animation
{
	class Material final
	{
	private:
		std::unique_ptr<class Renderer> &m_Renderer;

		float m_Shininess{32.0f};
		float m_SpecularStrength{0.5f};
		glm::vec3 m_DiffuseColor{-1.0f};

		std::unique_ptr<Shader> m_Shader{};
		std::unique_ptr<Texture> m_Texture{};

	public:
		Material();

		Material(const std::filesystem::path &sceneFile, const aiScene *scene, const aiMaterial *material);

		void start() const;

		void update(Uniform::UniformData uniformData) const;

		void end() const;

	private:
		std::unique_ptr<Texture> getDiffuseTexture(const std::filesystem::path &scenePath,
												   const aiScene *scene, const aiMaterial *material) const;

		std::unique_ptr<Texture> getEmbeddedTexture(const aiTexture *embeddedTexture) const;

		[[nodiscard]] std::unique_ptr<Texture> getExternalTexture(const std::filesystem::path &texturePath) const;

		void storeMaterialProperties(const aiMaterial *material);
	};
}

#endif //SKELETALANIMATION_MATERIAL_H
