#ifndef SKELETALANIMATION_SHADER_H
#define SKELETALANIMATION_SHADER_H

namespace Animation
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void startShader() = 0;

		virtual void stopShader() = 0;
	};
}

#endif //SKELETALANIMATION_SHADER_H
