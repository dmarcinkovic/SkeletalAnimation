#version 450

layout (binding = 1) uniform sampler2D texSampler;

layout (location = 0) in vec2 textureCoordinates;
layout (location = 1) in vec3 surfaceNormal;
layout (location = 2) flat in vec4 lightColor;
layout (location = 3) flat in vec3 cameraPosition;
layout (location = 4) flat in vec3 lightPosition;
layout (location = 5) in vec3 worldPosition;

layout(location = 0) out vec4 outColor;

void main()
{
    vec3 normal = normalize(surfaceNormal);

    float ambientFactor = 0.2f;
    vec3 ambient = lightColor.rgb * ambientFactor;

    vec3 lightDirection = -normalize(lightPosition);
    float diffuseFactor = max(dot(normal, lightDirection), 0.0f);
    vec3 diffuse = lightColor.rgb * diffuseFactor;

    vec3 toCameraVector = normalize(cameraPosition - worldPosition);
    vec3 reflectedVector = reflect(-lightDirection, normal);
    // TODO: pass this 32 as uniform and store it in material class
    float specularFactor = pow(max(dot(reflectedVector, toCameraVector), 0.0f), 32);
    // TODO: pass this 0.5f as an uniform and store it in material class
    vec3 specular = 0.5f * specularFactor * lightColor.rgb;

    vec3 objectColor = texture(texSampler, textureCoordinates).rgb;
    vec3 resultColor = (ambient + diffuse + specular) * objectColor;
    outColor = vec4(resultColor, 1.0f);
}
