#ifndef SKELETALANIMATION_WINDOW_H
#define SKELETALANIMATION_WINDOW_H

#include <GLFW/glfw3.h>
#include <string>

namespace Animation
{
	class Window final
	{
	private:
		static constexpr int WIDTH = 1200;
		static constexpr int HEIGHT = 800;
		static constexpr const char *TITLE = "Skeletal Animation";

		GLFWwindow *m_Window{};

	public:
		static Window &getWindow();

		~Window();

		Window(const Window &) = delete;

		[[nodiscard]] bool isRunning() const;

		static void clearWindow();

		void update();

		GLFWwindow *getRawWindow();

		void getFramebufferSize(int &width, int &height) const;

		void getValidFramebufferSize(int &width, int &height) const;

		static inline constexpr const char *getWindowTitle()
		{
			return TITLE;
		}

	private:
		Window(const char *title, int width, int height);

		static void setWindowHints();

		void createWindow(const char *title, int width, int height);

		static void windowResizeCallback(GLFWwindow *, int width, int height);
	};
}

#endif //SKELETALANIMATION_WINDOW_H
