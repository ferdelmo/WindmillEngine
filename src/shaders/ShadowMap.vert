#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform MVP {
    mat4 model;
    mat4 view;
    mat4 proj;
} mvpDepth;


// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 normal_modelspace;
layout(location = 4) in vec3 tangent_modelspace;
layout(location = 5) in vec3 bitangent_modelspace;

void main() {
	 gl_Position = mvpDepth.proj * mvpDepth.view * mvpDepth.model * vec4(vertexPosition_modelspace, 1.0);
}