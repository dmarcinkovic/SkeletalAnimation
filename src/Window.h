#ifndef SKELETALANIMATION_WINDOW_H
#define SKELETALANIMATION_WINDOW_H

#include <GLFW/glfw3.h>
#include <string>

namespace Animation
{
	class Window final
	{
	private:
		GLFWwindow *m_Window{};

	public:
		static Window &create(const char *title, int width, int height);

		~Window();

		Window(const Window &) = delete;

		[[nodiscard]] bool isRunning() const;

		static void clearWindow();

		void update();

	private:
		Window(const char *title, int width, int height);

		static void setWindowHints();

		void createWindow(const char *title, int width, int height);

		static void windowResizeCallback(GLFWwindow *, int width, int height);
	};
}

#endif //SKELETALANIMATION_WINDOW_H
