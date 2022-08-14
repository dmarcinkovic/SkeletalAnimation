#ifndef SKELETALANIMATION_RENDERER_H
#define SKELETALANIMATION_RENDERER_H

#include <memory>
#include <glm/vec4.hpp>

#include "Mesh.h"

namespace Animation
{
	class Renderer
	{
	protected:
		const glm::vec4 m_ClearColor{1.0f, 1.0f, 1.0f, 1.0f};
		std::vector<Mesh> m_Meshes;

	public:
		void render() const;

		void addMesh(Mesh mesh);

		static std::unique_ptr<Renderer> &getRenderer();

		virtual ~Renderer() = default;

		virtual void setViewport(int width, int height) = 0;

		virtual void clearWindow() = 0;

		virtual void draw(std::uint64_t vertexCount) const = 0;

		[[nodiscard]] virtual std::unique_ptr<VertexArrayObject> createVertexArrayObject() const = 0;

		[[nodiscard]] virtual std::unique_ptr<VertexBufferObject> createVertexBufferObject() const = 0;
	};
}

#endif //SKELETALANIMATION_RENDERER_H
