#ifndef SKELETALANIMATION_RENDEREROPENGL_H
#define SKELETALANIMATION_RENDEREROPENGL_H

#include "Renderer.h"

namespace Animation
{
	class RendererOpenGL final : public Renderer
	{
	private:
		int m_Width{};
		int m_Height{};

	public:
		static std::unique_ptr<Renderer> &getRenderer();

		RendererOpenGL(const RendererOpenGL &) = delete;

		~RendererOpenGL() override = default;

		void setViewport(int width, int height) override;

		void clearWindow() override;

		void draw(std::uint64_t vertexCount) const override;

		[[nodiscard]] std::unique_ptr<VertexArrayObject> createVertexArrayObject() const override;

		[[nodiscard]] std::unique_ptr<VertexBufferObject> createVertexBufferObject() const override;

		[[nodiscard]] std::unique_ptr<Shader> getShader() const override;

		[[nodiscard]] std::unique_ptr<Texture> getTexture(const std::filesystem::path &path) const override;

		[[nodiscard]] float getWindowAspectRatio() const;

	private:
		RendererOpenGL();
	};
}

#endif //SKELETALANIMATION_RENDEREROPENGL_H
