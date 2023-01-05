#ifndef SKELETALANIMATION_RENDERER_H
#define SKELETALANIMATION_RENDERER_H

#include <memory>
#include <glm/vec4.hpp>

#include "Shader.h"
#include "Texture.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

namespace Animation
{
	class Renderer
	{
	protected:
		const glm::vec4 m_ClearColor{0.7f, 0.7f, 0.7f, 1.0f};

	public:
		enum class RendererType
		{
			OPENGL,
			VULKAN
		};

		static std::unique_ptr<Renderer> &getRenderer();

		static void setRendererType(RendererType type);

		static void setWindowFlags();

		virtual ~Renderer() = default;

		virtual void preRender();

		virtual void postRender();

		virtual void setViewport(int width, int height) = 0;

		virtual void clearWindow() = 0;

		virtual void draw(std::uint64_t vertexCount) const = 0;

		[[nodiscard]] virtual std::unique_ptr<VertexArrayObject> createVertexArrayObject() const = 0;

		[[nodiscard]] virtual std::unique_ptr<VertexBufferObject> createVertexBufferObject() const = 0;

		[[nodiscard]] virtual std::unique_ptr<Shader> getShader() const = 0;

		[[nodiscard]] virtual std::unique_ptr<Texture> getTexture(const std::filesystem::path &path) const = 0;

		[[nodiscard]] virtual std::unique_ptr<Texture> getTexture(const std::uint8_t *data, int len) const = 0;

		[[nodiscard]] virtual std::unique_ptr<Texture> getTexture(const std::uint8_t *pixels, int w, int h) const = 0;

		[[nodiscard]] virtual glm::mat4 getProjectionMatrix(float fov, float near, float far) const = 0;
	};
}

#endif //SKELETALANIMATION_RENDERER_H
