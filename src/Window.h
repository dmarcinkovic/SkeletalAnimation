#ifndef SKELETALANIMATION_WINDOW_H
#define SKELETALANIMATION_WINDOW_H

#include <GLFW/glfw3.h>
#include <string>

namespace Animation
{
	class Window
	{
	private:
		GLFWwindow *m_Window{};

	public:
		Window(const char *title, int width, int height);

		~Window();

		[[nodiscard]] bool isRunning() const;

		static void clearWindow();

		void update();

	private:
		static void setWindowHints();

		void createWindow(const char *title, int width, int height);
	};
}

#endif //SKELETALANIMATION_WINDOW_H
