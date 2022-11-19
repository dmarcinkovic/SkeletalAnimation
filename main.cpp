#include <memory>

#include "Renderer.h"
#include "Window.h"
#include "Scene.h"

int main()
{
	Animation::Window &window = Animation::Window::getWindow();

	std::unique_ptr<Animation::Renderer> &renderer = Animation::Renderer::getRenderer();

	Animation::Scene::createScene("/home/david/untitled.obj");

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
