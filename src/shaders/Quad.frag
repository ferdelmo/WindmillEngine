#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec2 texCoord; 


layout(binding = 1) uniform sampler2D shadowMap;

layout(location = 0) out vec4 outColor;

float LinearizeDepth(float depth)
{
  float n = 0.1; // camera z near
  float f = 1000; // camera z far
  float z = depth;
  return (2.0 * n) / (f + n - z * (f - n));	
}

void main() {
	
	float depth = texture(shadowMap, texCoord).r;

	outColor = vec4(vec3(1.0-LinearizeDepth(depth)), 1.0);
}