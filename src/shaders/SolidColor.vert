#version 450
#extension GL_ARB_separate_shader_objects : enable



layout(binding = 0) uniform MVP {
    mat4 model;
    mat4 view;
    mat4 proj;
} mvp;

layout(binding = 1) uniform UniformColor {
	vec4 color;
} color;

layout(location = 0) in vec3 position;




layout(location = 0) out vec4 fragColor;

void main() {
    gl_Position = mvp.proj * mvp.view * mvp.model * vec4(position, 1.0);
    fragColor = color.color;
}