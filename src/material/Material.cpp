#include <cassert>
#include <assimp/scene.h>
#include <spdlog/spdlog.h>

#include "Material.h"
#include "Texture.png.h"
#include "Renderer.h"

namespace Animation
{
	Material::Material()
			: m_Renderer(Renderer::getRenderer()), m_Shader(m_Renderer->getShader()),
			  m_Texture(m_Renderer->getTexture(texture_png, static_cast<int>(texture_png_len)))
	{
		m_Shader->setTexture(m_Texture);
	}

	Material::Material(const std::filesystem::path &sceneFile, const aiScene *scene, const aiMaterial *material)
			: m_Renderer(Renderer::getRenderer()), m_Shader(m_Renderer->getShader())
	{
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			m_Texture = getDiffuseTexture(sceneFile, scene, material);
		} else
		{
			aiColor3D color;
			material->Get(AI_MATKEY_COLOR_DIFFUSE, color);

			m_DiffuseColor = glm::vec3{color.r, color.g, color.b};
			m_Texture = m_Renderer->getTexture(texture_png, static_cast<int>(texture_png_len));
		}

		storeMaterialProperties(material);
		m_Shader->setTexture(m_Texture);
	}

	void Material::start() const
	{
		assert(m_Shader);
		m_Shader->startShader();
	}

	void Material::update() const
	{
		m_Shader->update(this);
	}

	void Material::end() const
	{
		assert(m_Shader);
		m_Shader->stopShader();
	}

	float Material::getShininess() const
	{
		return m_Shininess;
	}

	float Material::getSpecularStrength() const
	{
		return m_SpecularStrength;
	}

	const glm::vec3 &Material::getDiffuseColor() const
	{
		return m_DiffuseColor;
	}

	std::unique_ptr<Texture> Material::getDiffuseTexture(const std::filesystem::path &scenePath,
														 const aiScene *scene, const aiMaterial *material) const
	{
		aiString path;
		aiReturn result = material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		assert(result == aiReturn_SUCCESS);

		const aiTexture *embeddedTexture = scene->GetEmbeddedTexture(path.C_Str());
		if (embeddedTexture)
		{
			return getEmbeddedTexture(embeddedTexture);
		}

		std::filesystem::path texturePath = scenePath.parent_path() / path.C_Str();
		return getExternalTexture(texturePath);
	}

	std::unique_ptr<Texture> Material::getEmbeddedTexture(const aiTexture *embeddedTexture) const
	{
		auto width = static_cast<int>(embeddedTexture->mWidth);
		auto height = static_cast<int>(embeddedTexture->mHeight);

		if (height == 0)
		{
			auto *data = reinterpret_cast<std::uint8_t *>(embeddedTexture->pcData);
			return m_Renderer->getTexture(data, width);
		}

		auto *pixels = reinterpret_cast<std::uint8_t *>(embeddedTexture->pcData);
		return m_Renderer->getTexture(pixels, width, height);
	}

	std::unique_ptr<Texture> Material::getExternalTexture(const std::filesystem::path &texturePath) const
	{
		if (!std::filesystem::exists(texturePath))
		{
			spdlog::error("Texture at location: {} does not exist.", absolute(texturePath).string());
			std::exit(EXIT_FAILURE);
		}

		return m_Renderer->getTexture(texturePath);
	}

	void Material::storeMaterialProperties(const aiMaterial *material)
	{
		float shininess;
		if (material->Get(AI_MATKEY_SHININESS, shininess) == aiReturn_SUCCESS)
		{
			m_Shininess = shininess;
		}

		float specularStrength;
		if (material->Get(AI_MATKEY_SHININESS_STRENGTH, specularStrength) == aiReturn_SUCCESS)
		{
			m_SpecularStrength = specularStrength;
		}
	}
}