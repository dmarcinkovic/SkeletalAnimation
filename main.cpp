#include "Renderer.h"
#include "Window.h"
#include "Scene.h"

int main()
{
	Animation::Window &window = Animation::Window::getWindow();

	Animation::Scene scene = Animation::Scene::createScene("/home/david/Downloads/Blender/Dying.fbx");

	while (window.isRunning())
	{
		Animation::Window::clearWindow();

		scene.updateAnimation();
		scene.render();

		window.update();
	}

	return 0;
}
