#include <vector>
#include <memory>

#include "RendererOpenGL.h"
#include "Window.h"
#include "Mesh.h"

int main()
{
	Animation::Window &window = Animation::Window::getWindow();

	std::unique_ptr<Animation::Renderer> &renderer = Animation::Renderer::getRenderer();

	std::vector<float> vertices{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.0f
	};

	std::vector<std::uint32_t> indices{0, 1, 2, 1, 3, 2};

	Animation::MeshData triangle(vertices, indices);
	Animation::Material material(renderer->getShader());

	renderer->addMesh(Animation::Mesh(std::move(triangle), std::move(material)));

	while (window.isRunning())
	{
		Animation::Window::clearWindow();

		renderer->preRender();
		renderer->render();
		renderer->postRender();

		window.update();
	}

	return 0;
}
