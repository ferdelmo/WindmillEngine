#version 450
#extension GL_ARB_separate_shader_objects : enable



layout(binding = 0) uniform MVP {
    mat4 model;
    mat4 view;
    mat4 proj;
} mvp;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 normal;

layout(location = 0) out vec3 normalFrag;
layout(location = 1) out vec3 worldPos;

void main() {
    gl_Position = mvp.proj * mvp.view * mvp.model * vec4(position, 1.0);
    normalFrag = (mvp.model * vec4(normal, 0.0)).xyz;
    worldPos =  (mvp.model * vec4(position, 1.0)).xyz;
}