#ifndef SKELETALANIMATION_RENDEREROPENGL_H
#define SKELETALANIMATION_RENDEREROPENGL_H

#include "Renderer.h"

namespace Animation
{
	class RendererOpenGL final : public Renderer
	{
	public:
		static std::unique_ptr<Renderer> &getRenderer();

		RendererOpenGL(const RendererOpenGL &) = delete;

		~RendererOpenGL() override = default;

		void setViewport(int width, int height) override;

		void clearWindow() override;

		void draw(std::uint64_t vertexCount) const override;

		[[nodiscard]] std::unique_ptr<VertexArrayObject> createVertexArrayObject() const override;

		[[nodiscard]] std::unique_ptr<VertexBufferObject> createVertexBufferObject() const override;

	private:
		RendererOpenGL();
	};
}

#endif //SKELETALANIMATION_RENDEREROPENGL_H
