#include <vector>
#include <memory>

#include "VertexArrayObjectOpenGL.h"
#include "VertexBufferObjectOpenGL.h"
#include "ShaderOpenGL.h"
#include "RendererOpenGL.h"
#include "Window.h"

int main()
{
	Animation::Window &window = Animation::Window::create("Skeletal Animation", 1200, 800);

	std::unique_ptr<Animation::Renderer> &renderer = Animation::RendererOpenGL::getRenderer();

	std::vector<float> vertices{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
	};

	std::unique_ptr<Animation::VertexArrayObject> vao = std::make_unique<Animation::VertexArrayObjectOpenGL>();
	std::unique_ptr<Animation::VertexBufferObject> verticesObject = std::make_unique<Animation::VertexBufferObjectOpenGL>();

	verticesObject->storeFloatData(vertices, 3);
	vao->storeData(0, std::move(verticesObject));

	std::unique_ptr<Animation::Shader> shader = std::make_unique<Animation::ShaderOpenGL>();

	while (window.isRunning())
	{
		Animation::Window::clearWindow();

		shader->startShader();
		vao->bind();

		glDrawArrays(GL_TRIANGLES, 0, 3);

		vao->unbind();
		shader->stopShader();

		window.update();
	}

	return 0;
}
