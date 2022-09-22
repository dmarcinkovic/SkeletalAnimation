#ifndef SKELETALANIMATION_MATERIAL_H
#define SKELETALANIMATION_MATERIAL_H

#include <memory>

#include "Shader.h"

namespace Animation
{
	class Material final
	{
	private:
		std::unique_ptr<Shader> m_Shader;

	public:
		// TODO: add texture and remove explicit keyword
		explicit Material(std::unique_ptr<Shader> shader);

		[[nodiscard]] const std::unique_ptr<Shader> &getShader() const;
	};
}

#endif //SKELETALANIMATION_MATERIAL_H
