#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(binding = 0) uniform UniformColor {
	vec3 color;
} color;

layout(binding = 1) uniform MVP {
    mat4 model;
    mat4 view;
    mat4 proj;
} mvp;

layout(location = 0) in vec3 position;




layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = mvp.proj * mvp.view * mvp.model * vec4(position, 1.0);
    fragColor = color.color;
}