#include <vector>
#include <memory>
#include <spdlog/spdlog.h>

#include "Renderer.h"
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

	std::vector<float> textureCoordinates{
			0.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f
	};

	std::vector<float> normals{
			1.0f, 1.0, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 1.0f
	};

	std::vector<std::uint32_t> indices{0, 1, 2, 1, 3, 2};

	std::filesystem::path texturePath("/home/david/CLionProjects/OpenGLLibrary/res/texture.jpg");
	auto shader = renderer->getShader();
	auto texture = renderer->getTexture(texturePath);
	Animation::Material material(std::move(shader), std::move(texture));

	Animation::MeshData triangle(vertices, textureCoordinates, normals, indices);
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
