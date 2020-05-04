#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 normal_modelspace;


layout(location = 0) out vec3 worldPos;
layout(location = 1) out vec2 texCoord; 

void main() {
    gl_Position = vec4(position, 1.0);
    texCoord = inTexCoord;
}