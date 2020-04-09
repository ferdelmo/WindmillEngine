#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 worldPos;

struct PointLight{
	vec3 position;
	float power;
	vec3 color;
	float aux;
};

layout(binding = 1) uniform Lights{
	PointLight[10] lights;
	int num_lights;
} lightsStruct;


layout(binding = 2) uniform sampler2D texSampler;

layout(binding = 3) uniform AmbientLight{
	vec3 color;
	float coef;
} ambientLight;

layout(location = 0) out vec4 outColor;

void main() {


	vec3 norm = normalize(normal);

	PointLight[10] lights = lightsStruct.lights;

	vec3 texColor = texture(texSampler, fragTexCoord).rgb;
	vec3 lightColor = ambientLight.color*ambientLight.coef;
	for(int i=0; i<lightsStruct.num_lights;i++){

		vec3 lightDir = normalize(lights[i].position-worldPos);
		float diff = max(dot(norm, lightDir), 0.0);

		// need to check this distance
		float dist = distance(worldPos, lights[i].position)*10;
		if(dist<1){
			dist = 1;
		}


		float distCof = min(lights[i].power/(dist*dist),1.0);

		lightColor+= (lights[i].color * diff * distCof);
	}

    outColor = vec4(lightColor * texColor, 1.0);
}