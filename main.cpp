#include "Window.h"

int main()
{
	Animation::Window window("Skeletal Animation", 1200, 800);

	while (window.isRunning())
	{
		Animation::Window::clearWindow();


		window.update();
	}

	return 0;
}
