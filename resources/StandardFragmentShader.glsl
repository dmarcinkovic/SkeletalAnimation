#version 430

layout(location = 0) out vec4 outColor;

layout (binding = 0) uniform UniformBufferObject {
    vec4 color;
} uniformObject;

void main()
{
    outColor = uniformObject.color;
}
