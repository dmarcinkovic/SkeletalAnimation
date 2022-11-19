#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

layout (location = 0) out vec2 textureCoordinates;
layout (location = 1) out vec3 surfaceNormal;
layout (location = 2) flat out vec4 lightColor;
layout (location = 3) flat out vec3 cameraPosition;
layout (location = 4) flat out vec3 lightPosition;
layout (location = 5) out vec3 worldPosition;
layout (location = 6) flat out vec3 diffuseColor;
layout (location = 7) flat out float shininess;
layout (location = 8) flat out float specularStrength;

layout (binding = 0) uniform UniformBufferObject {
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;
    vec4 lightColor;
    vec3 cameraPosition;
    vec3 lightPosition;
    vec3 diffuseColor;
    float shininess;
    float specularStrength;
} uniformObject;

void main()
{
    mat4 modelMatrix = uniformObject.modelMatrix;
    mat4 viewMatrix = uniformObject.viewMatrix;
    mat4 projectionMatrix = uniformObject.projectionMatrix;
    vec4 fragmentPosition = modelMatrix * vec4(position, 1.0f);
    gl_Position = projectionMatrix * viewMatrix * fragmentPosition;

    textureCoordinates = texCoord;
    surfaceNormal = mat3(transpose(inverse(modelMatrix))) * normal;
    worldPosition = fragmentPosition.xyz;

    lightColor = uniformObject.lightColor;
    cameraPosition = uniformObject.cameraPosition;
    lightPosition = uniformObject.lightPosition;
    diffuseColor = uniformObject.diffuseColor;

    shininess = uniformObject.shininess;
    specularStrength = uniformObject.specularStrength;
}
