#include "Renderer.h"
#include "RendererOpenGL.h"

namespace Animation
{
	std::unique_ptr<Renderer> &Renderer::getRenderer()
	{
		// TODO: do not hardcode: find which render is better for given OS
		return RendererOpenGL::getRenderer();
	}
}