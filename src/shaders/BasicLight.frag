#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 worldPos;

layout(binding = 1) uniform PointLight{
	glm::vec3 position;
	glm::vec3 color;
	float potencia;
} light;

layout(location = 0) out vec4 outColor;

void main() {
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lights.position-worldPos);

	float diff = max(dot(norm, lightDir), 0.0);

    outColor = vec4(diff * lightDir * fragColor, 1.0);
}