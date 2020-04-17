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


layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 worldPos;
layout(location = 3) out vec3 EyeDirection_cameraspace; 
layout(location = 4) out vec3 Normal_cameraspace;  

void main() {
    gl_Position = mvp.proj * mvp.view * mvp.model * vec4(position, 1.0);
    worldPos =  (mvp.model * vec4(position, 1.0)).xyz;

	vec3 vertexPosition_cameraspace = ( mvp.view * mvp.model * vec4(position,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Normal of the the vertex, in camera space
	Normal_cameraspace = (transpose(inverse(mvp.view * mvp.model)) * vec4(normal,0)).xyz; 
	// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

    fragColor = inColor;
    fragTexCoord = inTexCoord;
}